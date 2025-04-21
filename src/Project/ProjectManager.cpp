#include "ProjectManager.h"
#include <iostream>
#include "Graphics/Graphics.h"
#include "network/Client.h"


ProjectManager::ProjectManager(Window& win,Client& client) : m_window(win),
    client(client)
{
    Logo = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/Alchemist.png");
    minimize = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/minimize.png");
    maximize = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/maximize.png");
    close = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/close.png");
	chatWindow_ = std::make_unique<ChatWindow>(client);
    LoadProjects();
}

void ProjectManager::Update(Graphics& gfx)
{
    if (selectedProjectIndex >= 0 && selectedProjectIndex < m_projects.size()) {
        m_projects[selectedProjectIndex]->Update(gfx, m_timer.Tick());
    }
}

void ProjectManager::Render(Graphics& gfx)
{
    if (chatWindow_) {
        chatWindow_->render();
    }
    if (selectedProjectIndex >= 0 && selectedProjectIndex < m_projects.size()) {
        m_projects[selectedProjectIndex]->Render(gfx);

    }
}

void ProjectManager::ShowMenuBar(Graphics& gfx)
{
    if (ImGui::BeginMainMenuBar())
    {
        ImTextureID tex_id_1 = Logo->GetSRV();
        if (ImGui::ImageButton(tex_id_1, ImVec2(20, 20)))
        {

        }
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New project"))
            {
            }
            if (ImGui::MenuItem("Load project"))
            {
                //serializer->Deserialize("flavian.json");
            }
            if (ImGui::MenuItem("Save Project"))
            {
                if (currentProject) {
                    currentProject->Save(); // Save the current project
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                PostMessage(get<0>(gfx.getWin()), WM_CLOSE, 0, 0);

            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("add Scene"))
            {
            }
            if (ImGui::MenuItem("add object"))
            {
            }

            ImGui::EndMenu();
        }
        ImGui::Separator();
        ImGui::SameLine(ImGui::GetWindowWidth() - 140);
        ImTextureID tex_id_2 = minimize->GetSRV();
        if (ImGui::ImageButton(tex_id_2, ImVec2(20, 20)))
        {
            ShowWindow(get<0>(gfx.getWin()), SW_MINIMIZE);
        }
        ImGui::SameLine();
        ImTextureID tex_id_3 = maximize->GetSRV();

        if (ImGui::ImageButton(tex_id_3, ImVec2(20, 20)))
        {
            ShowWindow(get<0>(gfx.getWin()), IsZoomed(get<0>(gfx.getWin())) ? SW_RESTORE : SW_MAXIMIZE);
        }
        ImGui::SameLine();
        ImTextureID tex_id_4 = close->GetSRV();
        if (ImGui::ImageButton(tex_id_4, ImVec2(20, 20)))
        {
            PostMessage(get<0>(gfx.getWin()), WM_CLOSE, 0, 0);
        }
        // Handle dragging
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows))
        {
            if (!g_isDragging)
            {
                g_isDragging = true;
                GetCursorPos(&g_dragStartPoint);
                RECT windowRect;
                GetWindowRect(get<0>(gfx.getWin()), &windowRect);
                g_windowStartPoint.x = windowRect.left;
                g_windowStartPoint.y = windowRect.top;
            }
        }
        else if (!ImGui::IsMouseDown(0))
        {
            g_isDragging = false;
        }

        if (g_isDragging)
        {
            POINT currentPos;
            GetCursorPos(&currentPos);
            int dx = currentPos.x - g_dragStartPoint.x;
            int dy = currentPos.y - g_dragStartPoint.y;

            SetWindowPos(get<0>(gfx.getWin()), NULL, g_windowStartPoint.x + dx, g_windowStartPoint.y + dy, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }



        ImGui::EndMainMenuBar();
    }
}

void ProjectManager::LoadProjects()
{
    m_projects.push_back(std::make_unique<Project>(m_window, "Project1", "C:Projects/Project1"));
}

void ProjectManager::ShowProjectWindow()
{
    if (!showProjectWindow) {
        return; // Do not render the window if the flag is false
    }

    // Set the window size and position
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Project Manager");

    // Section: Header
    ImGui::Text("Manage Your Projects");
    ImGui::Separator();

    // Section: Project List
    ImGui::Text("Available Projects:");
    ImGui::BeginChild("Project List", ImVec2(0, 150), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    for (int i = 0; i < m_projects.size(); ++i) {
        bool isSelected = (i == selectedProjectIndex);
        if (ImGui::Selectable(m_projects[i]->GetName().c_str(), isSelected)) {
            selectedProjectIndex = i; // Update the selected project index
        }
    }
    ImGui::EndChild();

    // Section: Project Actions
    ImGui::Separator();
    ImGui::Text("Actions:");
    if (ImGui::Button("Create New Project")) {
        ImGui::OpenPopup("Create Project");
    }
    ImGui::SameLine();
    if (ImGui::Button("Load Project") && GetSelectedProject()) {
        LoadSelectedProject(); // Load the selected project only when this button is clicked
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete Project") && GetSelectedProject()) {
        m_projects.erase(m_projects.begin() + selectedProjectIndex);
        selectedProjectIndex = -1; // Reset the selection
    }

    // Popup: Create New Project
    if (ImGui::BeginPopup("Create Project")) {
        static char name[128] = "";
        static char rootDir[128] = "";

        ImGui::InputText("Project Name", name, IM_ARRAYSIZE(name));
        ImGui::InputText("Root Directory", rootDir, IM_ARRAYSIZE(rootDir));

        if (ImGui::Button("Create")) {
            CreateNewProject(name, rootDir);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Section: Project Details
    ImGui::Separator();
    if (GetSelectedProject()) {
        ImGui::Text("Selected Project:");
        ImGui::Text("Name: %s", GetSelectedProject()->GetName().c_str());
        ImGui::Text("Path: %s", GetSelectedProject()->GetRootDirectory().c_str());
    }
    else {
        ImGui::Text("No project selected.");
    }

    ImGui::End();
}

void ProjectManager::CreateNewProject(const std::string& name, const std::string& rootDir)
{
    m_projects.push_back(std::make_unique<Project>(m_window, name, rootDir));
    std::cout << "New project created: " << name << " at " << rootDir << std::endl;
}

Project* ProjectManager::GetSelectedProject()
{
    if (selectedProjectIndex >= 0 && selectedProjectIndex < m_projects.size()) {
        return m_projects[selectedProjectIndex].get();
    }
    return nullptr;
}

void ProjectManager::LoadSelectedProject()
{
    if (selectedProjectIndex >= 0 && selectedProjectIndex < m_projects.size()) {
        currentProject = m_projects[selectedProjectIndex].get();
        currentProject->GetSceneManager()->Update(m_window.GetInstance(), m_timer.Tick());
        
        showProjectWindow = false; 
    }
}

void ProjectManager::ShowChatWindow()
{
        client.setMessageCallback([this](const std::string& message) {
            if (chatWindow_) {
                chatWindow_->addMessage("Server2: " + message);
            }
            });
 
}
