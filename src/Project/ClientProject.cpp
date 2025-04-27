#include "ClientProject.h"
#include <fstream>
#include <iostream>
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

// Existing constructor (keep this implementation)
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

void ClientProject::Load() {
    std::string projectFile = m_rootDirectory + "/" + m_name + ".json";
    std::ifstream file(projectFile);
    if (file.is_open()) {
        nlohmann::json j;
        file >> j;
        file.close();

        // Deserialize project data
        manager->DeserializeSceneManager(j);
        std::cout << "Project loaded from: " << projectFile << std::endl;
    }
    else {
        std::cerr << "Failed to open project file for loading: " << projectFile << std::endl;
    }
}

void ClientProject::Save() {
    // Ensure the directory exists
    fs::create_directories(m_rootDirectory);

    std::string projectFile = m_rootDirectory + "/" + m_name + ".json";
    std::ofstream file(projectFile);
    if (file.is_open()) {
        nlohmann::json j = manager->SerializeSceneManager(m_name, m_rootDirectory);
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

// Serialize the project to JSON for network transmission
std::string ClientProject::SerializeToJson() const {
    nlohmann::json projectJson = manager->SerializeSceneManager(m_name, m_rootDirectory);

    // Add project metadata
    projectJson["name"] = m_name;
    projectJson["id"] = m_projectId;
    projectJson["last_modified"] = std::chrono::system_clock::to_time_t(m_lastModified);

    return projectJson.dump();
}

// Update the project from network data
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

        // Clear the local modification flag since we just updated from server
        m_locallyModified = false;

        std::cout << "Project updated from network data: " << m_name << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error updating project from network: " << e.what() << std::endl;
        throw; // Rethrow to let the caller handle it
    }
}