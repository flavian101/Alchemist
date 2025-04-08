#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Project.h"
#include "window/Window.h"
#include "window/ImguiManager.h"
#include "Graphics/FrameTime.h"

#include "Graphics/Utilis.h" 



class ProjectManager {
public:
    ProjectManager(Window& win);
    void Update(Graphics& gfx);
    void Render(Graphics& gfx);
    void ShowMenuBar(Graphics& gfx);
    void LoadProjects();
    void ShowProjectWindow();
    void CreateNewProject(const std::string& name, const std::string& rootDir);
    Project* GetSelectedProject();
    void LoadSelectedProject(); // Add this line

private:
    Window& m_window;
    std::vector<std::unique_ptr<Project>> m_projects;
    int selectedProjectIndex = -1;
    Project* currentProject = nullptr; // Add this line
    FrameTime m_timer; // Add this line
    bool showSceneWindow = true;
    bool createScenePopup = false;
    bool g_isDragging = false;
    POINT g_dragStartPoint = { 0, 0 };
    POINT g_windowStartPoint = { 0, 0 };
    std::unique_ptr<Utils::Texture> Logo;
    std::unique_ptr<Utils::Texture> close;
    std::unique_ptr<Utils::Texture> minimize;
    std::unique_ptr<Utils::Texture> maximize;
};
