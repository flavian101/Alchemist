#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Project.h"
#include "window/Window.h"
#include "window/ImguiManager.h"
#include "Graphics/FrameTime.h"
#include "window/ChatWindow.h"
#include "Graphics/Utilis.h" 
#include "network/Client.h" // Include the Client header


class ProjectManager {
    struct ProjectEntry {
        std::string projectId;
        std::string name;
        std::string rootDir;  

    };
public:
    ProjectManager(Window& win,Client& client);
    void Update(Graphics& gfx);
    void Render(Graphics& gfx);
    void ShowMenuBar(Graphics& gfx);
    void LoadProjects();
    void ShowProjectWindow();
    void CreateNewProject(const std::string& name, const std::string& rootDir);
    Project* GetSelectedProject();
    void LoadSelectedProject();
    void SaveSelectedProjectToServer();

    void ShowChatWindow(); // New method to display the chat window
    

private:
    Window& m_window;
    Client& client;
    std::vector<std::unique_ptr<Project>> m_projects;
    std::vector<ProjectEntry> syncedProjects;
    int selectedProjectIndex = -1;
    bool showProjectWindow = true; 
    Project* currentProject = nullptr; 
    FrameTime m_timer; 
    bool showSceneWindow = true;
    bool createScenePopup = false;
    bool g_isDragging = false;
    POINT g_dragStartPoint = { 0, 0 };
    POINT g_windowStartPoint = { 0, 0 };
    std::unique_ptr<Utils::Texture> Logo;
    std::unique_ptr<Utils::Texture> close;
    std::unique_ptr<Utils::Texture> minimize;
    std::unique_ptr<Utils::Texture> maximize;
    std::unique_ptr<ChatWindow> chatWindow_; // ChatWindow instance
    float syncTimer = 0.0f;
    float syncInterval = 3.0f; // Every 3 seconds

};
