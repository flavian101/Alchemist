#pragma once
#include "Scene/ClientSceneManager.h"
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include "window/Window.h"
#include <filesystem>

namespace fs = std::filesystem;

class ClientProject {
public:
    // Existing constructor for local projects
    ClientProject(Window& win, const std::string& name, const std::string& rootDir);

    // New constructor for network-loaded projects
    ClientProject(Window& win, const std::string& name, const std::string& projectId, const std::string& jsonData);

    ~ClientProject();

    // Getters/Setters
    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

    const std::string& GetRootDirectory() const { return m_rootDirectory; }

    const std::string& GetProjectId() const { return m_projectId; }
    void SetProjectId(const std::string& id) { m_projectId = id; }

    ClientSceneManager* GetSceneManager() { return manager; }

    // Core functionality
    void Update(Graphics& gfx, float deltaTime);
    void Render(Graphics& gfx);
    void Load();
    void Save();

    // Network collaboration functions
    std::string SerializeToJson() const;
    void UpdateFromJson(const std::string& jsonData);

    // Modification tracking
    std::chrono::system_clock::time_point GetLastModifiedTime() const { return m_lastModified; }
    void UpdateLastModifiedTime() {
        m_lastModified = std::chrono::system_clock::now();
        m_locallyModified = true;
    }

    bool IsModifiedLocally() const { return m_locallyModified; }
    void SetModifiedLocally(bool modified) { m_locallyModified = modified; }

private:
    std::string m_name;
    std::string m_rootDirectory;  // Local file path
    std::string m_projectId;      // Server identifier
    ClientSceneManager* manager;

    // Collaboration tracking
    std::chrono::system_clock::time_point m_lastModified;
    bool m_locallyModified = false;
};