#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <filesystem>
#include "Scene/ClientSceneManager.h"
#include "datapack/json.hpp"

namespace fs = std::filesystem;
class Window;
class Graphics;
class Client;

class ClientProject {
public:
    // Existing constructors
    ClientProject(Window& win, const std::string& name, const std::string& rootDir);
    ClientProject(Window& win, const std::string& name, const std::string& projectId, const std::string& jsonData);
    ~ClientProject();

    // Existing methods
    void Update(Graphics& gfx, float deltaTime);
    void Render(Graphics& gfx);
    void Load();
    void Save();
    std::string SerializeToJson() const;
    void UpdateFromJson(const std::string& jsonData);

    // Getters/Setters
    const std::string& GetName() const { return m_name; }
    const std::string& GetRootDirectory() const { return m_rootDirectory; }
    const std::string& GetProjectId() const { return m_projectId; }
    void SetProjectId(const std::string& id) { m_projectId = id; }

    // Modification tracking
    std::chrono::system_clock::time_point GetLastModifiedTime() const { return m_lastModified; }
    void UpdateLastModifiedTime() {
        m_lastModified = std::chrono::system_clock::now();
        m_locallyModified = true;
    }
    bool  IsModifiedLocally() const { return m_locallyModified; }
    void SetLocallyModified(bool modified) { m_locallyModified = modified; }

    // Model management methods
    void AddModel(const std::string& modelPath);
    void RemoveModel(const std::string& modelPath);
    bool SyncModels(Client* networkClient);
    const std::vector<std::string>& GetModelPaths() const { return m_modelPaths; }
    void SetModelsModified(bool modified) { m_modelsModified = modified; }
    bool AreModelsModified() const { return m_modelsModified; }
    bool LoadModelsFromServer(Client* networkClient);
    std::string ConvertServerToLocalPath(const std::string& serverPath);

    // Diagnostic methods
    void CheckModelStatus();
    bool RepairModels(Client* networkClient);

private:
    std::string m_name;
    std::string m_rootDirectory;
    std::string m_projectId;
    ClientSceneManager* manager;
    std::chrono::system_clock::time_point m_lastModified;
    bool m_locallyModified;

    // Model tracking
    std::vector<std::string> m_modelPaths;
    bool m_modelsModified = false;
};