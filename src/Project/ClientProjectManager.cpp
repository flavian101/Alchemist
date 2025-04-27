#include "ClientProjectManager.h"
#include "Graphics/Graphics.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "imgui/imgui.h"
#include "Graphics/Utilis.h"

ClientProjectManager::ClientProjectManager(Window& win, Client* networkClient)
    : m_window(win),
    m_networkClient(networkClient),
    lastSyncTime(std::chrono::system_clock::now())
{
    Logo = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/Alchemist.png");
    minimize = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/minimize.png");
    maximize = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/maximize.png");
    close = std::make_unique<Utils::Texture>(win.GetInstance(), "Assets/textures/thumbnail/close.png");

    // Set up callback to handle project updates from server
    if (m_networkClient) {
        m_networkClient->setProjectUpdateCallback([this](const std::string& projectId, const std::string& jsonData) {
            this->OnProjectUpdatedFromServer(projectId, jsonData);
            });
    }

    showLoginWindow = true; // Show login dialog on startup
}

ClientProjectManager::~ClientProjectManager() {
    // Nothing specific to clean up
}

void ClientProjectManager::Update(Graphics& gfx)
{
    // Check if it's time to sync with the server
    if (isLoggedIn && m_networkClient) {
        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastSyncTime).count();

        if (elapsed >= syncIntervalSeconds) {
            SyncWithServer();
            lastSyncTime = now;
        }
    }

    if (currentProject) {
        currentProject->Update(gfx, m_timer.Tick());
    }
}

void ClientProjectManager::Render(Graphics& gfx)
{
    if (currentProject) {
        currentProject->Render(gfx);
    }
}

void ClientProjectManager::ShowMenuBar(Graphics& gfx)
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
                if (isLoggedIn) {
                    showProjectWindow = true;
                }
                else {
                    showLoginWindow = true;
                }
            }

            if (ImGui::MenuItem("Open projects")) {
                if (isLoggedIn) {
                    LoadProjectsFromServer();
                    showProjectWindow = true;
                }
                else {
                    showLoginWindow = true;
                }
            }

            if (ImGui::MenuItem("Save Project")) {
                if (currentProject && isLoggedIn && m_networkClient) {
                    // Save locally
                    currentProject->Save();
                    currentProject->UpdateLastModifiedTime();

                    // Upload to server
                    std::string projectId = currentProject->GetRootDirectory();
                    std::string name = currentProject->GetName();
                    std::string jsonData = currentProject->SerializeToJson();

                    m_networkClient->saveProject(projectId, name, jsonData);
                    std::cout << "Project saved to server: " << name << std::endl;
                }
                else if (!isLoggedIn) {
                    showLoginWindow = true;
                }
            }

            if (ImGui::MenuItem(isLoggedIn ? "Logout" : "Login")) {
                if (isLoggedIn) {
                    isLoggedIn = false;
                    currentUsername = "";
                    m_projects.clear();
                    currentProject = nullptr;
                }
                else {
                    showLoginWindow = true;
                }
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                PostMessage(get<0>(gfx.getWin()), WM_CLOSE, 0, 0);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Sync Now") && isLoggedIn) {
                SyncWithServer();
            }

            if (ImGui::MenuItem("Add Scene") && currentProject) {
                // Add scene functionality for the SceneManager
            }

            if (ImGui::MenuItem("Add Object") && currentProject) {
                // Add object functionality
            }

            ImGui::EndMenu();
        }

        // Display logged in user info
        if (isLoggedIn) {
            ImGui::SameLine(ImGui::GetWindowWidth() - 240);
            ImGui::Text("User: %s", currentUsername.c_str());
        }

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

    // Login window
    if (showLoginWindow) {
        ShowLoginDialog();
    }

    // Project window
    if (showProjectWindow) {
        ShowProjectWindow();
    }
}

void ClientProjectManager::LoadProjectsFromServer()
{
    if (!m_networkClient || !isLoggedIn) return;

    // Clear existing projects
    m_projects.clear();

    // Request project list from server
    m_networkClient->listProjects([this](const std::vector<std::pair<std::string, std::string>>& projects) {
        // Create Project objects for each project from the server
        for (const auto& [id, name] : projects) {
            // We initially create projects with just ID and name
            // The full data will be loaded when a project is selected
            auto newProject = std::make_unique<ClientProject>(m_window, name, id);
            newProject->SetProjectId(id);
            m_projects.push_back(std::move(newProject));
        }

        // Show project window after receiving projects
        showProjectWindow = true;
        });

    // Reset selection
    selectedProjectIndex = -1;
}

void ClientProjectManager::ShowProjectWindow()
{
    if (!showProjectWindow) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Project Manager", &showProjectWindow);

    // Refresh button to reload projects
    if (ImGui::Button("Refresh Project List")) {
        if (isLoggedIn) {
            LoadProjectsFromServer();
        }
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
            ImGui::Text("Project ID: %s", m_projects[i]->GetProjectId().c_str());
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

    // Popup: Create New Project
    static char nameBuffer[128] = "";
    static char dirBuffer[256] = "";

    if (ImGui::BeginPopup("Create Project")) {
        ImGui::Text("Create New Project");
        ImGui::Separator();

        ImGui::InputText("Project Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));
        ImGui::InputText("Project Directory", dirBuffer, IM_ARRAYSIZE(dirBuffer));

        if (ImGui::Button("Create", ImVec2(120, 0))) {
            if (strlen(nameBuffer) > 0) {
                // Create project on client side and then upload to server
                std::string projectId = GenerateUniqueId();
                std::string projectDir = strlen(dirBuffer) > 0 ? dirBuffer : "Projects/" + std::string(nameBuffer);

                // Create local project
                CreateNewProject(nameBuffer, projectDir);

                // Get newly created project and upload to server
                if (!m_projects.empty()) {
                    ClientProject* newProject = m_projects.back().get();
                    newProject->SetProjectId(projectId);

                    // Save project to server
                    m_networkClient->saveProject(projectId, nameBuffer, newProject->SerializeToJson());

                    memset(nameBuffer, 0, sizeof(nameBuffer));
                    memset(dirBuffer, 0, sizeof(dirBuffer));
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
            memset(dirBuffer, 0, sizeof(dirBuffer));
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Project Details
    ImGui::Separator();
    if (GetSelectedProject()) {
        ImGui::Text("Selected Project:");
        ImGui::Text("Name: %s", GetSelectedProject()->GetName().c_str());
        ImGui::Text("ID: %s", GetSelectedProject()->GetProjectId().c_str());
        ImGui::Text("Local Directory: %s", GetSelectedProject()->GetRootDirectory().c_str());
    }
    else {
        ImGui::Text("No project selected.");
    }

    ImGui::End();
}

void ClientProjectManager::LoadSelectedProject()
{
    if (selectedProjectIndex >= 0 && selectedProjectIndex < m_projects.size() && m_networkClient) {
        std::string projectId = m_projects[selectedProjectIndex]->GetProjectId();

        m_networkClient->loadProject(projectId, [this, projectId](const std::string& jsonData) {
            // Find the project in our collection
            for (auto& proj : m_projects) {
                if (proj->GetProjectId() == projectId) {
                    // Update the project with the full data
                    proj->UpdateFromJson(jsonData);

                    // Set as current project
                    currentProject = proj.get();
                    showProjectWindow = false;
                    std::cout << "Project loaded from server: " << currentProject->GetName() << std::endl;
                    break;
                }
            }
            });
    }
}

void ClientProjectManager::CreateNewProject(const std::string& name, const std::string& rootDir)
{
    m_projects.push_back(std::make_unique<ClientProject>(m_window, name, rootDir));
    std::cout << "New project created: " << name << " at " << rootDir << std::endl;
}

ClientProject* ClientProjectManager::GetSelectedProject()
{
    if (selectedProjectIndex >= 0 && selectedProjectIndex < m_projects.size()) {
        return m_projects[selectedProjectIndex].get();
    }
    return nullptr;
}

std::string ClientProjectManager::GenerateUniqueId()
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

void ClientProjectManager::ShowLoginDialog()
{
    ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Login", &showLoginWindow, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    static char username[128] = "";
    static char password[128] = "";
    static bool loginFailed = false;

    ImGui::Text("Login to Collaboration Server");
    ImGui::Separator();

    ImGui::InputText("Username", username, IM_ARRAYSIZE(username));
    ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

    if (loginFailed) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "Login failed. Please try again.");
    }

    if (ImGui::Button("Login", ImVec2(100, 0))) {
        if (m_networkClient && m_networkClient->authenticate(username, password)) {
            isLoggedIn = true;
            currentUsername = username;
            loginFailed = false;
            showLoginWindow = false;

            // Load available projects after login
            LoadProjectsFromServer();
        }
        else {
            loginFailed = true;
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Register", ImVec2(100, 0))) {
        // Show registration window instead
        // For simplicity, we'll just call register directly
        if (m_networkClient && strlen(username) > 0 && strlen(password) > 0) {
            if (m_networkClient->registerUser(username, password)) {
                ImGui::OpenPopup("Registration Success");
            }
            else {
                ImGui::OpenPopup("Registration Failed");
            }
        }
    }

    // Registration success popup
    if (ImGui::BeginPopupModal("Registration Success", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Registration successful. You can now login.");
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    // Registration failed popup
    if (ImGui::BeginPopupModal("Registration Failed", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Registration failed. Username may already exist.");
        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

void ClientProjectManager::SyncWithServer()
{
    if (!isLoggedIn || !m_networkClient || !currentProject) {
        return;
    }

    // 1. Check if we have local changes to push
    if (currentProject->IsModifiedLocally()) {
        std::string projectId = currentProject->GetProjectId();
        std::string name = currentProject->GetName();
        std::string jsonData = currentProject->SerializeToJson();

        // Send changes to server
        m_networkClient->pushProject(projectId, name, jsonData);

        // Reset modified flag
        currentProject->SetModifiedLocally(false);
        std::cout << "Project pushed to server: " << name << std::endl;
    }

    // 2. Refresh project list to check for any new projects
    LoadProjectsFromServer();
}

void ClientProjectManager::OnProjectUpdatedFromServer(const std::string& projectId, const std::string& jsonData)
{
    // Find the project in our collection
    for (auto& proj : m_projects) {
        if (proj->GetProjectId() == projectId) {
            // Only update if this is our current project and we're not already modifying it locally
            if (currentProject == proj.get() && !currentProject->IsModifiedLocally()) {
                proj->UpdateFromJson(jsonData);
                std::cout << "Project updated from server: " << proj->GetName() << std::endl;
            }
            return;
        }
    }

    // If we get here, this is a new project we don't have locally
    // We should add it to our collection
    // First, parse the JSON to get the project name
    try {
        nlohmann::json projJson = nlohmann::json::parse(jsonData);
        if (projJson.contains("name")) {
            std::string name = projJson["name"];

            // Create a new project with the data
            auto newProject = std::make_unique<ClientProject>(m_window, name, projectId, jsonData);
            newProject->SetProjectId(projectId);
            m_projects.push_back(std::move(newProject));

            std::cout << "New project received from server: " << name << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing project update: " << e.what() << std::endl;
    }
}
void ClientProjectManager::ShowChatWindow() {
    if (!currentProject) return;

    ImGui::Begin("Project Chat");

    // Display chat history
    ImGui::BeginChild("ChatHistory", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
    for (const auto& msg : chatHistory) {
        ImGui::TextWrapped("%s", msg.c_str());
    }
    ImGui::EndChild();

    // Input for new messages
    static std::string chatMessage;

    char buffer[256];  // Temporary buffer with fixed size
    strncpy(buffer, chatMessage.c_str(), sizeof(buffer));  // Copy string to buffer
    buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination

    if (ImGui::InputText("##chatInput", buffer, IM_ARRAYSIZE(buffer))) {
        chatMessage = buffer;  // Update chatMessage from buffer
    }

    ImGui::SameLine();
    if (ImGui::Button("Send") || ImGui::IsKeyPressed(ImGuiKey_Enter)) {
        if (!chatMessage.empty()) {
            SendChatMessage(chatMessage);
            chatMessage.clear();
        }
    }

    ImGui::End();
}

void ClientProjectManager::SendChatMessage(const std::string& message) {
    if (m_networkClient && currentProject) {
        // Add a prefix to identify this as a chat message for the current project
        std::string formattedMessage = "CHAT_PROJECT " + currentProject->GetProjectId() + " " + message;
        m_networkClient->sendMessageToServer(formattedMessage);

        // Add to local history
        std::string displayMessage = currentUsername + ": " + message;
        chatHistory.push_back(displayMessage);
    }
}