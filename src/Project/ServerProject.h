#pragma once
#include "scene/ServerSceneManager.h"
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include "window/Window.h"
#include <filesystem> // C++17

namespace fs = std::filesystem;

class Graphics;

class ServerProject {
public:
    // Constructor for creating a new project
    ServerProject(Window& win, const std::string& name, const std::string& rootDir);

    // Constructor for loading a project from database/JSON
    ServerProject(Window& win, const std::string& name, const std::string& projectId, const std::string& jsonData);

    // Destructor
    ~ServerProject();

    // Basic getters/setters
    const std::string& GetName() const;
    void SetName(const std::string& name) { m_name = name; }

    const std::string& GetRootDirectory() const;

    const std::string& GetProjectId() const { return m_projectId; }
    void SetProjectId(const std::string& id) { m_projectId = id; }

    ServerSceneManager* GetSceneManager() { return manager; }

    // Core functionality
    void Update(Graphics& gfx, float deltaTime);
    void Render(Graphics& gfx);
    void Load();
    void Save();

    // Collaboration functions
    std::string SerializeToJson() const;
    void UpdateFromJson(const std::string& jsonData);

    // Modification tracking
    std::chrono::system_clock::time_point GetLastModifiedTime() const { return m_lastModified; }
    void UpdateLastModifiedTime() { m_lastModified = std::chrono::system_clock::now(); }

private:
    std::string m_name;
    std::string m_rootDirectory;
    std::string m_projectId;      // Server identifier
    ServerSceneManager* manager;

    // Collaboration tracking
    std::chrono::system_clock::time_point m_lastModified;
};