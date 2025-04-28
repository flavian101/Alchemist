#include "ServerProject.h"
#include <fstream>
#include <iostream>
#include "Scene/GameObject.h"
#include "Scene/Scene.h"
#include "Graphics/Graphics.h"
#include "ServerProjectManager.h"
#include "network/NetworkServer.h"

// Constructor for creating a new project
ServerProject::ServerProject(Window& win, const std::string& name, const std::string& rootDir)
    : m_name(name),
    m_rootDirectory(rootDir),
    m_projectId(rootDir),  // Use rootDir as projectId by default
    manager(new ServerSceneManager(&win)),
    m_lastModified(std::chrono::system_clock::now())
{
    // The ServerSceneManager constructor already creates a default scene
}

// Constructor for loading a project from database/JSON
ServerProject::ServerProject(Window& win, const std::string& name, const std::string& projectId, const std::string& jsonData)
    : m_name(name),
    m_rootDirectory("ServerProjects/" + projectId),
    m_projectId(projectId),
    manager(new ServerSceneManager(&win)),
    m_lastModified(std::chrono::system_clock::now())
{
    // Create the project directory if it doesn't exist
    fs::create_directories(m_rootDirectory);

    // Parse the JSON data
    if (!jsonData.empty()) {
        try {
            UpdateFromJson(jsonData);
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing project data: " << e.what() << std::endl;
            // The ServerSceneManager constructor already creates a default scene
        }
    }
}

// Destructor
ServerProject::~ServerProject()
{
    delete manager;
}

// Getter for the project name
const std::string& ServerProject::GetName() const
{
    return m_name;
}

// Getter for the root directory
const std::string& ServerProject::GetRootDirectory() const
{
    return m_rootDirectory;
}

// Update the project (called per frame)
void ServerProject::Update(Graphics& gfx, float deltaTime)
{
    if (manager) {
        manager->Update(gfx, deltaTime);
    }
}

// Render the project (called per frame)
void ServerProject::Render(Graphics& gfx)
{
    if (manager) {
        manager->Render(gfx);
    }
}

// Load the project from a file
void ServerProject::Load()
{
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

// Save the project to a file
void ServerProject::Save()
{
    // Ensure the directory exists
    fs::create_directories(m_rootDirectory);

    std::string projectFile = m_rootDirectory + "/" + m_name + ".json";
    std::ofstream file(projectFile);
    if (file.is_open()) {
        nlohmann::json j = manager->SerializeSceneManager(m_name, m_rootDirectory);

        // Add model paths
        j["models"] = m_modelPaths;

        file << j.dump(4);
        file.close();

        // Update modification time
        m_lastModified = std::chrono::system_clock::now();
        std::cout << "Project saved to: " << projectFile << std::endl;
    }
    else {
        std::cerr << "Failed to open project file for saving: " << projectFile << std::endl;
    }
}
std::vector<std::string> ServerProject::GetModelPaths() const {
    return m_modelPaths;
}

// Serialize the project to JSON for database storage
std::string ServerProject::SerializeToJson() const
{
    nlohmann::json projectJson = manager->SerializeSceneManager(m_name, m_rootDirectory);

    // Add project metadata
    projectJson["name"] = m_name;
    projectJson["id"] = m_projectId;

    // Convert the timestamp to a number
    auto timeT = std::chrono::system_clock::to_time_t(m_lastModified);
    projectJson["last_modified"] = timeT;

    // Add model paths
    projectJson["models"] = m_modelPaths;

    return projectJson.dump();
}

// Update the project from JSON data
void ServerProject::UpdateFromJson(const std::string& jsonData)
{
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

        std::cout << "Project updated from JSON data: " << m_name << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error updating project from JSON: " << e.what() << std::endl;
        throw; // Rethrow to let the caller handle it
    }
}

bool ServerProject::LoadModelsFromClient(const std::string& clientRootDir) {
    bool success = true;

    try {
        // First, ensure our model paths are up-to-date from the project file
        Load();

        // Copy each model from client path to server path
        for (const auto& modelPath : m_modelPaths) {
            std::string clientPath = clientRootDir + "/" + modelPath;
            std::string serverPath = m_rootDirectory + "/" + modelPath;

            // Create server directories
            fs::path serverDir = fs::path(serverPath).parent_path();
            fs::create_directories(serverDir);

            // Check if client file exists
            if (fs::exists(clientPath)) {
                // Copy file
                try {
                    fs::copy_file(clientPath, serverPath, fs::copy_options::overwrite_existing);
                    std::cout << "Copied model from " << clientPath << " to " << serverPath << std::endl;
                }
                catch (const std::exception& e) {
                    std::cerr << "Failed to copy model: " << e.what() << std::endl;
                    success = false;
                }
            }
            else {
                std::cerr << "Client model not found: " << clientPath << std::endl;
                success = false;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading models from client: " << e.what() << std::endl;
        success = false;
    }

    return success;
}

// Method to convert client path to server path
std::string ServerProject::ConvertClientToServerPath(const std::string& clientPath, const std::string& clientRootDir) {
    // Client path format: "[clientRootDir]/[relativePath]"
    // Server path format: "[m_rootDirectory]/[relativePath]"

    // Extract the relative path part
    if (clientPath.find(clientRootDir + "/") == 0) {
        std::string relativePath = clientPath.substr(clientRootDir.length() + 1);
        return m_rootDirectory + "/" + relativePath;
    }

    // If not a client path, return as is
    return clientPath;
}
void ServerProject::AddModel(const std::string& modelPath) {
    // Check if model already exists in paths
    auto it = std::find(m_modelPaths.begin(), m_modelPaths.end(), modelPath);
    if (it == m_modelPaths.end()) {
        m_modelPaths.push_back(modelPath);
        // No need to set modified flag - server projects don't track this
        std::cout << "Added model to project: " << modelPath << std::endl;
    }
}