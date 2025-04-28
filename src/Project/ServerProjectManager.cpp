#include "ServerProjectManager.h"
#include "Graphics/Graphics.h"
#include "network/NetworkServer.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "imgui/imgui.h"
#include "Graphics/Utilis.h"

ServerProjectManager::ServerProjectManager(Window& win, DatabaseManager& dbManager, NetworkServer* server)
    : m_window(win),
    m_dbManager(dbManager),
    m_server(server)
{
    Logo = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/Alchemist.png");
    minimize = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/minimize.png");
    maximize = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/maximize.png");
    close = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/close.png");

    // Load projects from the database when the manager is created
    LoadProjectsFromDatabase();
}

ServerProjectManager::~ServerProjectManager() {
    // Clean up if needed
}

void ServerProjectManager::Update(Graphics& gfx)
{
    if (currentProject) {
        currentProject->Update(gfx, m_timer.Tick());
    }
}

void ServerProjectManager::Render(Graphics& gfx)
{
    if (currentProject) {
        currentProject->Render(gfx);
    }
}

void ServerProjectManager::ShowMenuBar(Graphics& gfx)
{
    if (ImGui::BeginMainMenuBar())
    {
        ImTextureID tex_id_1 = Logo->GetSRV();
        if (ImGui::ImageButton(tex_id_1, ImVec2(20, 20))) {
            // Logo button action
        }

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New project")) {
                showProjectWindow = true;
            }

            if (ImGui::MenuItem("Open projects")) {
                LoadProjectsFromDatabase();
                showProjectWindow = true;
            }

            if (ImGui::MenuItem("Save Project")) {
                if (currentProject) {
                    currentProject->Save();

                    // Update project in database
                    std::string projectId = currentProject->GetRootDirectory();
                    std::string name = currentProject->GetName();
                    std::string jsonData = currentProject->SerializeToJson();

                    m_dbManager.storeProject(projectId, name, "admin", jsonData);
                    std::cout << "Project saved to database: " << name << std::endl;
                }
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                PostMessage(get<0>(gfx.getWin()), WM_CLOSE, 0, 0);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Server"))
        {
            if (ImGui::MenuItem("Manage Collaborators")) {
                showCollaboratorsWindow = true;
            }

            ImGui::EndMenu();
        }

        ImGui::Separator();

        // Window controls
        ImGui::SameLine(ImGui::GetWindowWidth() - 140);
        ImTextureID tex_id_2 = minimize->GetSRV();
        if (ImGui::ImageButton(tex_id_2, ImVec2(20, 20))) {
            ShowWindow(get<0>(gfx.getWin()), SW_MINIMIZE);
        }

        ImGui::SameLine();
        ImTextureID tex_id_3 = maximize->GetSRV();
        if (ImGui::ImageButton(tex_id_3, ImVec2(20, 20))) {
            ShowWindow(get<0>(gfx.getWin()), IsZoomed(get<0>(gfx.getWin())) ? SW_RESTORE : SW_MAXIMIZE);
        }

        ImGui::SameLine();
        ImTextureID tex_id_4 = close->GetSRV();
        if (ImGui::ImageButton(tex_id_4, ImVec2(20, 20))) {
            PostMessage(get<0>(gfx.getWin()), WM_CLOSE, 0, 0);
        }

        // Handle dragging
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows)) {
            if (!g_isDragging) {
                g_isDragging = true;
                GetCursorPos(&g_dragStartPoint);
                RECT windowRect;
                GetWindowRect(get<0>(gfx.getWin()), &windowRect);
                g_windowStartPoint.x = windowRect.left;
                g_windowStartPoint.y = windowRect.top;
            }
        }
        else if (!ImGui::IsMouseDown(0)) {
            g_isDragging = false;
        }

        if (g_isDragging) {
            POINT currentPos;
            GetCursorPos(&currentPos);
            int dx = currentPos.x - g_dragStartPoint.x;
            int dy = currentPos.y - g_dragStartPoint.y;

            SetWindowPos(get<0>(gfx.getWin()), NULL, g_windowStartPoint.x + dx, g_windowStartPoint.y + dy, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }

        ImGui::EndMainMenuBar();
    }

    // Collaborator management window
    if (showCollaboratorsWindow) {
        ShowCollaboratorAssignment();
    }
}

void ServerProjectManager::LoadProjectsFromDatabase()
{
    // Clear existing projects
    m_projects.clear();

    // For the server, get ALL projects from the database
    try {
        SQLite::Statement query(m_dbManager.getDatabase(),
            "SELECT project_id, name FROM projects;");

        while (query.executeStep()) {
            std::string id = query.getColumn(0).getText();
            std::string name = query.getColumn(1).getText();

            auto projectData = m_dbManager.loadProject(id);
            if (projectData.has_value()) {
                // Create project with database data
                m_projects.push_back(std::make_unique<ServerProject>(m_window, name, id, projectData.value()));
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load projects: " << e.what() << std::endl;
    }

    // Reset selection
    selectedProjectIndex = -1;
}

void ServerProjectManager::ShowProjectWindow()
{
    if (!showProjectWindow) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Project Manager", &showProjectWindow);

    // Refresh button to reload projects
    if (ImGui::Button("Refresh Project List")) {
        LoadProjectsFromDatabase();
    }

    // Project List
    ImGui::Text("Available Projects:");
    ImGui::BeginChild("Project List", ImVec2(0, 150), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    for (int i = 0; i < m_projects.size(); ++i) {
        bool isSelected = (i == selectedProjectIndex);
        if (ImGui::Selectable(m_projects[i]->GetName().c_str(), isSelected)) {
            selectedProjectIndex = i;
        }

        // Show project details on hover
        if (ImGui::IsItemHovered() && ImGui::BeginTooltip()) {
            ImGui::Text("Project ID: %s", m_projects[i]->GetRootDirectory().c_str());

            // Show collaborators
            auto collabIter = m_projectCollaborators.find(m_projects[i]->GetRootDirectory());
            if (collabIter != m_projectCollaborators.end()) {
                ImGui::Text("Collaborators:");
                for (const auto& collab : collabIter->second) {
                    ImGui::Text("  %s (%s)", collab.username.c_str(), collab.role.c_str());
                }
            }

            ImGui::EndTooltip();
        }
    }
    ImGui::EndChild();

    // Project Actions
    ImGui::Separator();
    ImGui::Text("Actions:");
    if (ImGui::Button("Create New Project")) {
        ImGui::OpenPopup("Create Project");
    }
    ImGui::SameLine();

    bool canLoadProject = (selectedProjectIndex >= 0 && selectedProjectIndex < m_projects.size());
    if (ImGui::Button("Load Selected Project", ImVec2(150, 0)) && canLoadProject) {
        LoadSelectedProject();
    }

    ImGui::SameLine();
    if (ImGui::Button("Delete Project") && canLoadProject) {
        // TODO: Implement project deletion from database
        m_projects.erase(m_projects.begin() + selectedProjectIndex);
        selectedProjectIndex = -1;
    }

    ImGui::SameLine();
    if (ImGui::Button("Manage Collaborators") && canLoadProject) {
        showCollaboratorsWindow = true;
    }

    // Popup: Create New Project
    static char nameBuffer[128] = "";
    static char descBuffer[256] = "";

    if (ImGui::BeginPopup("Create Project")) {
        ImGui::Text("Create New Project");
        ImGui::Separator();

        ImGui::InputText("Project Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));
        if (ImGui::Button("Create", ImVec2(120, 0))) {
            if (strlen(nameBuffer) > 0) {
                if (CreateProjectInDatabase(nameBuffer)) {
                    memset(nameBuffer, 0, sizeof(nameBuffer));
                    ImGui::CloseCurrentPopup();
                }

            }
            else {
                // Show error about empty name
                ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Project name cannot be empty!");
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            memset(nameBuffer, 0, sizeof(nameBuffer));
            memset(descBuffer, 0, sizeof(descBuffer));
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Project Details
    ImGui::Separator();
    if (GetSelectedProject()) {
        ImGui::Text("Selected Project:");
        ImGui::Text("Name: %s", GetSelectedProject()->GetName().c_str());
        ImGui::Text("ID: %s", GetSelectedProject()->GetRootDirectory().c_str());

        // You could add more details here from the project
    }
    else {
        ImGui::Text("No project selected.");
    }

    ImGui::End();
}

void ServerProjectManager::LoadSelectedProject()
{
    if (selectedProjectIndex >= 0 && selectedProjectIndex < m_projects.size()) {
        currentProject = m_projects[selectedProjectIndex].get();
        showProjectWindow = false;
    }
}

bool ServerProjectManager::CreateProjectInDatabase(const std::string& name)
{
    // Generate a unique project ID
    std::string projectId = GenerateUniqueId();


    ServerProject tempProject(m_window, name, projectId);
    std::string jsonData = tempProject.SerializeToJson();


    std::string currentUser = "admin";

    // Store in database
    bool success = m_dbManager.storeProject(projectId, name, currentUser, jsonData);

    if (success) {
        // Add to local collection as well
        m_projects.push_back(std::make_unique<ServerProject>(m_window, name, projectId, jsonData));
        std::cout << "Created new project in database: " << name << " (ID: " << projectId << ")" << std::endl;


        // Set initial collaborator (admin as owner)
        AddCollaborator(projectId, currentUser, "owner");
    }

    return success;
}

ServerProject* ServerProjectManager::GetSelectedProject()
{
    if (selectedProjectIndex >= 0 && selectedProjectIndex < m_projects.size()) {
        return m_projects[selectedProjectIndex].get();
    }
    return nullptr;
}

std::string ServerProjectManager::GenerateUniqueId()
{
    // A simple implementation - in production use a proper UUID library
    static int counter = 0;

    // Create a timestamp string
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "proj_" << std::put_time(std::localtime(&time_t_now), "%Y%m%d%H%M%S") << "_" << counter++;

    return ss.str();
}

void ServerProjectManager::ShowCollaboratorAssignment()
{
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Manage Collaborators", &showCollaboratorsWindow);

    // Project selection
    if (m_projects.empty()) {
        ImGui::Text("No projects available. Create a project first.");
        ImGui::End();
        return;
    }

    // Project dropdown
    static int projectIndex = 0;
    if (ImGui::BeginCombo("Project", m_projects[projectIndex]->GetName().c_str())) {
        for (int i = 0; i < m_projects.size(); i++) {
            bool isSelected = (projectIndex == i);
            if (ImGui::Selectable(m_projects[i]->GetName().c_str(), isSelected)) {
                projectIndex = i;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    std::string projectId = m_projects[projectIndex]->GetRootDirectory();

    // Current collaborators list
    ImGui::Text("Current Collaborators:");
    ImGui::BeginChild("Collaborators", ImVec2(0, 150), true);

    auto collabs = GetCollaborators(projectId);
    for (const auto& collab : collabs) {
        ImGui::Text("%s (%s)", collab.username.c_str(), collab.role.c_str());
        ImGui::SameLine(ImGui::GetWindowWidth() - 60);
        if (ImGui::Button(("Remove##" + collab.username).c_str(), ImVec2(50, 0))) {
            RemoveCollaborator(projectId, collab.username);
        }
    }

    ImGui::EndChild();

    // Add new collaborator
    ImGui::Separator();
    ImGui::Text("Add New Collaborator:");

    static char username[64] = "";
    static int roleIndex = 0;
    const char* roles[] = { "viewer", "editor", "owner" };

    ImGui::InputText("Username", username, IM_ARRAYSIZE(username));
    ImGui::Combo("Role", &roleIndex, roles, IM_ARRAYSIZE(roles));

    if (ImGui::Button("Add Collaborator")) {
        if (strlen(username) > 0) {
            AddCollaborator(projectId, username, roles[roleIndex]);
            memset(username, 0, sizeof(username));
        }
    }

    ImGui::End();
}

void ServerProjectManager::AddCollaborator(const std::string& projectId, const std::string& username, const std::string& role)
{
    // Add to our in-memory tracking
    auto& projectCollabs = m_projectCollaborators[projectId];

    // Check if user already exists
    auto it = std::find_if(projectCollabs.begin(), projectCollabs.end(),
        [&username](const CollaboratorInfo& info) {
            return info.username == username;
        });

    if (it != projectCollabs.end()) {
        // Update existing role
        it->role = role;
    }
    else {
        // Add new collaborator
        projectCollabs.push_back({ username, role });
    }

    // Update in database (project_users table)
    // This would be a SQL query to insert or update the collaborator
    // For example:
    // INSERT INTO project_users (project_id, username, role) VALUES (?, ?, ?)
    // ON CONFLICT (project_id, username) DO UPDATE SET role = ?

    try {
        SQLite::Statement query(m_dbManager.getDatabase(),
            "INSERT OR REPLACE INTO project_users (project_id, username, role) VALUES (?, ?, ?);");
        query.bind(1, projectId);
        query.bind(2, username);
        query.bind(3, role);
        query.exec();

        std::cout << "Added collaborator " << username << " with role " << role << " to project " << projectId << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to add collaborator: " << e.what() << std::endl;
    }
}

void ServerProjectManager::RemoveCollaborator(const std::string& projectId, const std::string& username)
{
    // Remove from in-memory tracking
    auto& projectCollabs = m_projectCollaborators[projectId];
    projectCollabs.erase(
        std::remove_if(projectCollabs.begin(), projectCollabs.end(),
            [&username](const CollaboratorInfo& info) {
                return info.username == username;
            }),
        projectCollabs.end());

    // Remove from database
    try {
        SQLite::Statement query(m_dbManager.getDatabase(),
            "DELETE FROM project_users WHERE project_id = ? AND username = ?;");
        query.bind(1, projectId);
        query.bind(2, username);
        query.exec();

        std::cout << "Removed collaborator " << username << " from project " << projectId << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to remove collaborator: " << e.what() << std::endl;
    }
}

std::vector<ServerProjectManager::CollaboratorInfo> ServerProjectManager::GetCollaborators(const std::string& projectId)
{
    // Check if we already have them in memory
    auto it = m_projectCollaborators.find(projectId);
    if (it != m_projectCollaborators.end()) {
        return it->second;
    }

    // Otherwise, load from database
    std::vector<CollaboratorInfo> collaborators;

    try {
        SQLite::Statement query(m_dbManager.getDatabase(),
            "SELECT username, role FROM project_users WHERE project_id = ?;");
        query.bind(1, projectId);

        while (query.executeStep()) {
            std::string username = query.getColumn(0).getText();
            std::string role = query.getColumn(1).getText();
            collaborators.push_back({ username, role });
        }

        // Cache in memory for future use
        m_projectCollaborators[projectId] = collaborators;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to load collaborators: " << e.what() << std::endl;
    }

    return collaborators;
}