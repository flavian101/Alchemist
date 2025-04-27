#pragma once
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include "ClientProject.h"
#include "graphics/FrameTime.h"
#include "network/Client.h"

class Window;
class Graphics;

// Client-side project management class
class ClientProjectManager
{
public:
    ClientProjectManager(Window& win, Client* networkClient);
    ~ClientProjectManager();

    void Update(Graphics& gfx);
    void Render(Graphics& gfx);
    void ShowMenuBar(Graphics& gfx);

    // Project management
    void LoadProjectsFromServer();
    void ShowProjectWindow();
    void ToggleProjectWindow() { showProjectWindow = !showProjectWindow; }
    bool IsProjectWindowOpen() const { return showProjectWindow; }

    // Network synchronization
    void SyncWithServer();
    void OnProjectUpdatedFromServer(const std::string& projectId, const std::string& data);
    void SetSyncInterval(int seconds) { syncIntervalSeconds = seconds; }

    // Project operations
    void CreateNewProject(const std::string& name, const std::string& rootDir);
    ClientProject* GetSelectedProject();
    void LoadSelectedProject();
    std::string GenerateUniqueId();

    // Authentication
    void ShowLoginDialog();
    bool IsLoggedIn() const { return isLoggedIn; }
    const std::string& GetCurrentUsername() const { return currentUsername; }

private:
    std::vector<std::unique_ptr<ClientProject>> m_projects;
    Window& m_window;
    Client* m_networkClient;
    ClientProject* currentProject = nullptr;
    FrameTime m_timer;

    // Authentication state
    bool isLoggedIn = false;
    std::string currentUsername;

    // Synchronization
    std::chrono::system_clock::time_point lastSyncTime;
    int syncIntervalSeconds = 10;  // Default sync every 10 seconds

    // UI state
    bool showProjectWindow = false;
    bool showLoginWindow = false;
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

public:
    void ShowChatWindow();
    bool HasSelectedProject() const { return currentProject != nullptr; }

private:
    // Chat functionality
    std::string chatMessage;
    std::vector<std::string> chatHistory;
    void SendChatMessage(const std::string& message);
};