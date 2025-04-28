#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "ServerProject.h"
#include "Graphics/FrameTime.h"
#include "network/DatabaseManager.h"

class Window;
class Graphics;
class NetworkServer;

// Server-side project management class
class ServerProjectManager
{
public:
    ServerProjectManager(Window& win, DatabaseManager& dbManager, NetworkServer* server);
    ~ServerProjectManager();

    void Update(Graphics& gfx);
    void Render(Graphics& gfx);
    void ShowMenuBar(Graphics& gfx);

    // Project management
    void LoadProjectsFromDatabase();
    void ShowProjectWindow();
    void ToggleProjectWindow() { showProjectWindow = !showProjectWindow; }
    bool IsProjectWindowOpen() const { return showProjectWindow; }

    // Collaborator management
    struct CollaboratorInfo {
        std::string username;
        std::string role;  // "owner", "editor", "viewer"
    };
    int GetProjectCount() const {
        return static_cast<int>(m_projects.size());
    }

    void ShowCollaboratorAssignment();
    void AddCollaborator(const std::string& projectId, const std::string& username, const std::string& role);
    void RemoveCollaborator(const std::string& projectId, const std::string& username);
    std::vector<CollaboratorInfo> GetCollaborators(const std::string& projectId);

    // Project operations
    bool CreateProjectInDatabase(const std::string& name);
    ServerProject* GetSelectedProject();
    void LoadSelectedProject();
    std::string GenerateUniqueId();
    void BroadcastProjectUpdate(const std::string& projectId, const std::string& jsonData);

    void HandleModelUpload(const std::string& projectId, const std::string& modelPath, const std::vector<char>& modelData);

    void BroadcastModelUpdate(const std::string& projectId, const std::string& modelPath);

private:
    std::vector<std::unique_ptr<ServerProject>> m_projects;
    Window& m_window;
    DatabaseManager& m_dbManager;
    NetworkServer* m_server;
    ServerProject* currentProject = nullptr;
    FrameTime m_timer;

    // Collaboration
    std::unordered_map<std::string, std::vector<CollaboratorInfo>> m_projectCollaborators;

    // UI state
    bool showProjectWindow = false;
    bool showCollaboratorsWindow = false;
    int selectedProjectIndex = -1;

    // Window dragging
    bool g_isDragging = false;
    POINT g_dragStartPoint = { 0, 0 };
    POINT g_windowStartPoint = { 0, 0 };

    // UI resources
    std::unique_ptr<Utils::Texture> Logo;
    std::unique_ptr<Utils::Texture> minimize;
    std::unique_ptr<Utils::Texture> maximize;
    std::unique_ptr<Utils::Texture> close;
};