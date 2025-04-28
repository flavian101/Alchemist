#include "ClientProject.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "network/Client.h"

ClientProject::ClientProject(Window& win, const std::string& name, const std::string& rootDir)
    : m_name(name), m_rootDirectory(rootDir), m_projectId(""),
    manager(new ClientSceneManager(&win)),
    m_lastModified(std::chrono::system_clock::now()),
    m_locallyModified(false)
{
    // Initialize with a default scene
    Scene* defaultScene = new Scene("Default", win);
    manager->AddScene(defaultScene);
}

// New constructor for network-loaded projects
ClientProject::ClientProject(Window& win, const std::string& name, const std::string& projectId, const std::string& jsonData)
    : m_name(name), m_rootDirectory("Projects/" + name), m_projectId(projectId),
    manager(new ClientSceneManager(&win)),
    m_lastModified(std::chrono::system_clock::now()),
    m_locallyModified(false)
{
    // Create the project directory if it doesn't exist
    fs::create_directories(m_rootDirectory);

    // Parse the JSON data from the server and initialize the project
    if (!jsonData.empty()) {
        try {
            UpdateFromJson(jsonData);
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing project data: " << e.what() << std::endl;
            // Create a default scene as fallback
            Scene* defaultScene = new Scene("Default", win);
            manager->AddScene(defaultScene);
        }
    }
    else {
        // Create a default scene if no data provided
        Scene* defaultScene = new Scene("Default", win);
        manager->AddScene(defaultScene);
    }
}

ClientProject::~ClientProject()
{
    delete manager;
}
void ClientProject::Update(Graphics& gfx, float deltaTime)
{
    if (manager) {
        manager->Update(gfx, deltaTime);
    }
}

void ClientProject::Render(Graphics& gfx)
{
    if (manager) {
        manager->Render(gfx);
    }
}
// Update SerializeToJson to include model information
std::string ClientProject::SerializeToJson() const {
    nlohmann::json projectJson = manager->SerializeSceneManager(m_name, m_rootDirectory);

    // Add project metadata
    projectJson["name"] = m_name;
    projectJson["id"] = m_projectId;
    projectJson["last_modified"] = std::chrono::system_clock::to_time_t(m_lastModified);

    // Add model paths
    projectJson["models"] = m_modelPaths;

    return projectJson.dump();
}

// Update UpdateFromJson to handle model information
void ClientProject::UpdateFromJson(const std::string& jsonData) {
    try {
        nlohmann::json projectJson = nlohmann::json::parse(jsonData);

        // Update project metadata if available
        if (projectJson.contains("name")) {
            m_name = projectJson["name"];
        }

        if (projectJson.contains("id")) {
            m_projectId = projectJson["id"];
        }

        // Deserialize scene data
        manager->DeserializeSceneManager(projectJson);

        // Update last modified time
        if (projectJson.contains("last_modified")) {
            std::time_t timestamp = projectJson["last_modified"];
            m_lastModified = std::chrono::system_clock::from_time_t(timestamp);
        }
        else {
            m_lastModified = std::chrono::system_clock::now();
        }

        // Update model paths if available
        if (projectJson.contains("models") && projectJson["models"].is_array()) {
            m_modelPaths.clear();
            for (const auto& model : projectJson["models"]) {
                m_modelPaths.push_back(model);
            }
        }

        // Clear the local modification flag since we just updated from server
        m_locallyModified = false;
        m_modelsModified = false;

        std::cout << "Project updated from network data: " << m_name << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error updating project from network: " << e.what() << std::endl;
        throw; // Rethrow to let the caller handle it
    }
}

// Update Load method to handle models
void ClientProject::Load() {
    std::string projectFile = m_rootDirectory + "/" + m_name + ".json";
    std::ifstream file(projectFile);
    if (file.is_open()) {
        nlohmann::json j;
        file >> j;
        file.close();

        // Deserialize project data
        manager->DeserializeSceneManager(j);

        // Load model paths from project file
        if (j.contains("models") && j["models"].is_array()) {
            m_modelPaths.clear();
            for (const auto& model : j["models"]) {
                m_modelPaths.push_back(model);
            }
        }

        std::cout << "Project loaded from: " << projectFile << std::endl;
    }
    else {
        std::cerr << "Failed to open project file for loading: " << projectFile << std::endl;
    }
}

// Update Save method to include models
void ClientProject::Save() {
    // Ensure the directory exists
    fs::create_directories(m_rootDirectory);

    std::string projectFile = m_rootDirectory + "/" + m_name + ".json";
    std::ofstream file(projectFile);
    if (file.is_open()) {
        nlohmann::json j = manager->SerializeSceneManager(m_name, m_rootDirectory);

        // Add model paths
        j["models"] = m_modelPaths;

        file << j.dump(4); // Pretty print with 4-space indentation
        file.close();

        // Update modification tracking
        UpdateLastModifiedTime();
        std::cout << "Project saved to: " << projectFile << std::endl;
    }
    else {
        std::cerr << "Failed to open project file for saving: " << projectFile << std::endl;
    }
}

// Model management implementations
void ClientProject::AddModel(const std::string& modelPath) {
    // Check if model already exists
    auto it = std::find(m_modelPaths.begin(), m_modelPaths.end(), modelPath);
    if (it == m_modelPaths.end()) {
        m_modelPaths.push_back(modelPath);
        m_modelsModified = true;
        m_locallyModified = true;
        std::cout << "Added model to project: " << modelPath << std::endl;
    }
}

void ClientProject::RemoveModel(const std::string& modelPath) {
    auto it = std::find(m_modelPaths.begin(), m_modelPaths.end(), modelPath);
    if (it != m_modelPaths.end()) {
        m_modelPaths.erase(it);
        m_modelsModified = true;
        m_locallyModified = true;
        std::cout << "Removed model from project: " << modelPath << std::endl;
    }
}

bool ClientProject::SyncModels(Client* networkClient) {
    if (!networkClient || m_projectId.empty()) return false;

    bool success = true;

    // For each model, check if it exists locally and upload if needed
    for (const auto& modelPath : m_modelPaths) {
        fs::path fullPath = fs::path(m_rootDirectory) / modelPath;

        if (fs::exists(fullPath)) {
            // Read the model file
            std::ifstream modelFile(fullPath, std::ios::binary);
            if (!modelFile) {
                std::cerr << "Failed to open model file: " << fullPath << std::endl;
                success = false;
                continue;
            }

            // Get file size
            modelFile.seekg(0, std::ios::end);
            size_t fileSize = modelFile.tellg();
            modelFile.seekg(0, std::ios::beg);

            // Read file content
            std::vector<char> buffer(fileSize);
            if (!modelFile.read(buffer.data(), fileSize)) {
                std::cerr << "Failed to read model file: " << fullPath << std::endl;
                success = false;
                continue;
            }

            // Upload to server
            networkClient->uploadModel(m_projectId, modelPath, buffer.data(), fileSize);
            std::cout << "Uploaded model to server: " << modelPath << std::endl;
        }
    }

    // Reset modification flag if all uploads were successful
    if (success) {
        m_modelsModified = false;
    }

    return success;
}

bool ClientProject::LoadModelsFromServer(Client* networkClient) {
    if (!networkClient || m_projectId.empty()) {
        return false;
    }

    bool success = true;
    std::vector<std::string> pendingDownloads;

    // First check which models we need to download
    for (const auto& modelPath : m_modelPaths) {
        std::string localPath = m_rootDirectory + "/" + modelPath;

        // Check if the model exists locally
        if (!fs::exists(localPath)) {
            pendingDownloads.push_back(modelPath);
        }
    }

    // If all models are already downloaded, return success
    if (pendingDownloads.empty()) {
        return true;
    }

    // Set up a counter to track download completion
    std::atomic<int> pendingCount(pendingDownloads.size());
    std::atomic<bool> allSuccess(true);
    std::mutex downloadMutex;
    std::condition_variable downloadCV;

    // Download each missing model
    for (const auto& modelPath : pendingDownloads) {
        networkClient->downloadModel(m_projectId, modelPath,
            [&, localPath = m_rootDirectory + "/" + modelPath, modelPath](const std::vector<char>& data) {
                // Create directories if needed
                fs::path dirPath = fs::path(localPath).parent_path();
                fs::create_directories(dirPath);

                // Save the model file
                std::ofstream modelFile(localPath, std::ios::binary);
                if (modelFile) {
                    modelFile.write(data.data(), data.size());
                    modelFile.close();
                    std::cout << "Downloaded model: " << modelPath << std::endl;
                }
                else {
                    std::cerr << "Failed to save model: " << localPath << std::endl;
                    allSuccess.store(false);
                }

                // Decrease pending count and notify if all downloads completed
                if (--pendingCount == 0) {
                    std::unique_lock<std::mutex> lock(downloadMutex);
                    downloadCV.notify_all();
                }
            });
    }

    // Wait for all downloads to complete (with timeout)
    std::unique_lock<std::mutex> lock(downloadMutex);
    if (!downloadCV.wait_for(lock, std::chrono::seconds(30), [&]() { return pendingCount == 0; })) {
        std::cerr << "Timeout waiting for model downloads to complete" << std::endl;
        return false;
    }

    return allSuccess.load();
}

std::string ClientProject::ConvertServerToLocalPath(const std::string& serverPath) {
    // Server path format: "ServerModels/[projectId]/[relativePath]"
    // Local path format: "[rootDirectory]/[relativePath]"

    // Extract the relative path part
    std::string serverPrefix = "ServerModels/" + m_projectId + "/";
    if (serverPath.find(serverPrefix) == 0) {
        std::string relativePath = serverPath.substr(serverPrefix.length());
        return m_rootDirectory + "/" + relativePath;
    }

    // If not a server path, return as is
    return serverPath;
}

// Diagnostic methods
void ClientProject::CheckModelStatus() {
    std::cout << "Checking model status for project: " << m_name << std::endl;
    std::cout << "Project ID: " << m_projectId << std::endl;
    std::cout << "Root directory: " << m_rootDirectory << std::endl;
    std::cout << "Total models: " << m_modelPaths.size() << std::endl;

    for (const auto& modelPath : m_modelPaths) {
        std::string localPath = m_rootDirectory + "/" + modelPath;
        bool exists = fs::exists(localPath);
        std::cout << "  Model: " << modelPath << " - "
            << (exists ? "Exists locally" : "Missing locally") << std::endl;
    }
}

bool ClientProject::RepairModels(Client* networkClient) {
    if (!networkClient || m_projectId.empty()) {
        std::cerr << "Cannot repair models: no network client or project ID" << std::endl;
        return false;
    }

    std::cout << "Repairing models for project: " << m_name << std::endl;

    // Force download all models regardless of local status
    bool success = true;
    std::atomic<int> pendingCount(m_modelPaths.size());
    std::mutex repairMutex;
    std::condition_variable repairCV;

    for (const auto& modelPath : m_modelPaths) {
        std::string localPath = m_rootDirectory + "/" + modelPath;

        // Delete existing file if it exists (to force a clean download)
        if (fs::exists(localPath)) {
            try {
                fs::remove(localPath);
            }
            catch (const std::exception& e) {
                std::cerr << "Failed to remove existing model: " << e.what() << std::endl;
            }
        }

        // Download from server
        networkClient->downloadModel(m_projectId, modelPath,
            [&, localPath, modelPath](const std::vector<char>& data) {
                // Create directories if needed
                fs::path dirPath = fs::path(localPath).parent_path();
                fs::create_directories(dirPath);

                // Save the model file
                std::ofstream modelFile(localPath, std::ios::binary);
                if (modelFile) {
                    modelFile.write(data.data(), data.size());
                    modelFile.close();
                    std::cout << "Repaired model: " << modelPath << std::endl;
                }
                else {
                    std::cerr << "Failed to repair model: " << localPath << std::endl;
                    success = false;
                }

                // Decrease pending count and notify if all repairs completed
                if (--pendingCount == 0) {
                    std::unique_lock<std::mutex> lock(repairMutex);
                    repairCV.notify_all();
                }
            });
    }

    // Wait for all repairs to complete (with timeout)
    std::unique_lock<std::mutex> lock(repairMutex);
    if (!repairCV.wait_for(lock, std::chrono::seconds(60), [&]() { return pendingCount == 0; })) {
        std::cerr << "Timeout waiting for model repairs to complete" << std::endl;
        return false;
    }

    return success;
}