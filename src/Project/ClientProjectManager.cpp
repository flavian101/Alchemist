#include "ClientProjectManager.h"
#include "Graphics/Graphics.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "imgui/imgui.h"
#include <fstream>
#include "window/Window.h"



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

        // Register model update callback
        m_networkClient->setModelUpdateCallback([this](const std::string& projectId, const std::string& modelPath) {
            this->OnModelUpdatedFromServer(projectId, modelPath);
            });

        // Register message callback for chat messages
        m_networkClient->setMessageCallback([this](const std::string& message) {
            // Handle incoming chat messages
            if (message.find("CHAT_PROJECT ") == 0) {
                size_t firstSpace = message.find(' ');
                size_t secondSpace = message.find(' ', firstSpace + 1);

                if (secondSpace != std::string::npos) {
                    std::string projectId = message.substr(firstSpace + 1, secondSpace - firstSpace - 1);
                    std::string content = message.substr(secondSpace + 1);

                    // Only add to chat history if this is for our current project
                    if (currentProject && currentProject->GetProjectId() == projectId) {
                        chatHistory.push_back(content);
                    }
                }
            }
            });
    }
}

ClientProjectManager::~ClientProjectManager() {
    // Nothing specific to clean up
}

void ClientProjectManager::SetAuthenticated(bool authenticated, const std::string& username) {
    isLoggedIn = authenticated;
    currentUsername = username;

    // When authenticated, immediately load projects
    if (isLoggedIn) {
        std::cout << "User " << currentUsername << " authenticated successfully, loading projects..." << std::endl;
        LoadProjectsFromServer();
    }
    else {
        // Clear projects when logged out
        m_projects.clear();
        currentProject = nullptr;
    }
}

void ClientProjectManager::Update(Graphics& gfx)
{
    // Check if it's time to sync with the server
    if (autoSyncEnabled && isLoggedIn && m_networkClient) {
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

    // Show conflict dialog if needed
    if (showConflictDialog) {
        ShowConflictResolutionDialog();
    }

    // Show chat window if needed
    if (currentProject && showChatWindow) {
        ShowChatWindow();
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
                    std::string projectId = currentProject->GetProjectId();
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
                    SetAuthenticated(false, "");
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

            // Add sync settings submenu
            if (ImGui::BeginMenu("Sync Settings")) {
                bool autoSync = IsAutoSyncEnabled();
                if (ImGui::Checkbox("Auto-Sync", &autoSync)) {
                    EnableAutoSync(autoSync);
                }

                int interval = GetSyncInterval();
                if (ImGui::SliderInt("Sync Interval (seconds)", &interval, 5, 60)) {
                    SetSyncInterval(interval);
                }

                ImGui::EndMenu();
            }

            // Add a separator before other menu items
            ImGui::Separator();

            if (ImGui::MenuItem("Add Scene") && currentProject) {
                // Add scene functionality for the SceneManager
                RecordChange("AddScene", "NewScene");
            }

            if (ImGui::MenuItem("Add Object") && currentProject) {
                // Add object functionality
                RecordChange("AddObject", "NewObject");
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Project Manager")) {
                showProjectWindow = true;
            }

            if (ImGui::MenuItem("Chat") && currentProject) {
                // Toggle chat window visibility
            }

            ImGui::EndMenu();
        }

        // Display logged in user info
        if (isLoggedIn) {
            ImGui::SameLine(ImGui::GetWindowWidth() - 240);
            ImGui::Text("User: %s", currentUsername.c_str());
        }

        // Add sync status indicator
        if (isLoggedIn && currentProject) {
            ImGui::SameLine(ImGui::GetWindowWidth() - 350);
            if (currentProject->IsModifiedLocally() || currentProject->AreModelsModified()) {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Local Changes*");
            }
            else {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "In Sync");
            }
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

    // Project window
    if (showProjectWindow) {
        ShowProjectWindow();
    }
}

void ClientProjectManager::LoadProjectsFromServer()
{
    if (!m_networkClient || !m_networkClient->isAuthenticated()) {
        std::cerr << "Cannot load projects: client not connected or not authenticated" << std::endl;
        return;
    }

    // Clear existing projects
    m_projects.clear();

    // Add a loading indicator or message
    std::cout << "Loading projects for user: " << currentUsername << "..." << std::endl;

    // Request project list from server
    m_networkClient->listProjects([this](const std::vector<std::pair<std::string, std::string>>& projects) {
        // Create Project objects for each project from the server
        for (const auto& [id, name] : projects) {
            auto newProject = std::make_unique<ClientProject>(m_window, name, id);
            newProject->SetProjectId(id);
            m_projects.push_back(std::move(newProject));
            std::cout << "Loaded project: " << name << " (ID: " << id << ")" << std::endl;
        }

        // Ensure UI is updated
        showProjectWindow = true;
        selectedProjectIndex = -1;  // Reset selection

        // If no projects were found, log it
        if (projects.empty()) {
            std::cout << "No projects found for user: " << currentUsername << std::endl;
        }
        else {
            std::cout << "Found " << projects.size() << " projects for user: " << currentUsername << std::endl;
        }
        });
}

void ClientProjectManager::ShowProjectWindow()
{
    if (!showProjectWindow) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Project Manager", &showProjectWindow);

    // User status
    if (isLoggedIn) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Logged in as: %s", currentUsername.c_str());
    }
    else {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Not logged in");
    }

    // Refresh button to reload projects
    if (ImGui::Button("Refresh Project List")) {
        if (isLoggedIn) {
            LoadProjectsFromServer();
        }
        else {
            showLoginWindow = true;
        }
    }

    // Project status information
    ImGui::SameLine();
    ImGui::Text("Total Projects: %d", static_cast<int>(m_projects.size()));

    // Project List with better visibility
    ImGui::Text("Available Projects:");
    ImGui::BeginChild("Project List", ImVec2(0, 150), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    if (m_projects.empty()) {
        if (isLoggedIn) {
            ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f),
                "No projects found for user: %s", currentUsername.c_str());
            ImGui::TextWrapped("You can create a new project using the 'Create New Project' button below.");
        }
        else {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                "You need to log in to view projects.");
        }
    }

    for (int i = 0; i < m_projects.size(); ++i) {
        bool isSelected = (i == selectedProjectIndex);
        std::string displayName = m_projects[i]->GetName();

        // If project ID is available, show a truncated version
        if (!m_projects[i]->GetProjectId().empty()) {
            displayName += " (ID: " + m_projects[i]->GetProjectId().substr(0, 8) + "...)";
        }

        if (ImGui::Selectable(displayName.c_str(), isSelected)) {
            selectedProjectIndex = i;
        }

        // Show project details on hover
        if (ImGui::IsItemHovered() && ImGui::BeginTooltip()) {
            ImGui::Text("Project ID: %s", m_projects[i]->GetProjectId().c_str());
            ImGui::Text("Project Name: %s", m_projects[i]->GetName().c_str());
            ImGui::Text("Local Directory: %s", m_projects[i]->GetRootDirectory().c_str());
            ImGui::EndTooltip();
        }
    }
    ImGui::EndChild();

    // Project Actions
    ImGui::Separator();
    ImGui::Text("Actions:");

    if (ImGui::Button("Create New Project") && isLoggedIn) {
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

    // Project Details section - add sync status and controls
    ImGui::Separator();
    if (GetSelectedProject()) {
        ImGui::Text("Selected Project:");
        ImGui::Text("Name: %s", GetSelectedProject()->GetName().c_str());
        ImGui::Text("ID: %s", GetSelectedProject()->GetProjectId().c_str());
        ImGui::Text("Local Directory: %s", GetSelectedProject()->GetRootDirectory().c_str());

        // Add sync status indicators
        if (isLoggedIn && m_networkClient) {
            if (GetSelectedProject()->IsModifiedLocally()) {
                ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Status: Modified Locally");
            }
            else {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Status: In Sync with Server");
            }

            // Add model count
            const auto& modelPaths = GetSelectedProject()->GetModelPaths();
            ImGui::Text("Models: %d", static_cast<int>(modelPaths.size()));

            // Add model management buttons
            if (ImGui::CollapsingHeader("Model Management")) {
                if (ImGui::Button("Check Model Status")) {
                    GetSelectedProject()->CheckModelStatus();
                }

                ImGui::SameLine();

                if (ImGui::Button("Repair Models")) {
                    GetSelectedProject()->RepairModels(m_networkClient);
                }

                // List models
                ImGui::BeginChild("ModelsList", ImVec2(0, 100), true);
                for (const auto& modelPath : modelPaths) {
                    std::string localPath = GetSelectedProject()->GetRootDirectory() + "/" + modelPath;
                    bool exists = fs::exists(localPath);

                    if (exists) {
                        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "%s", modelPath.c_str());
                    }
                    else {
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s (Missing)", modelPath.c_str());
                    }
                }
                ImGui::EndChild();
            }
        }
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

        std::cout << "Loading project from server: " << projectId << std::endl;

        m_networkClient->loadProject(projectId, [this, projectId](const std::string& jsonData) {
            // Find the project in our collection
            for (auto& proj : m_projects) {
                if (proj->GetProjectId() == projectId) {
                    // Update the project with the full data
                    proj->UpdateFromJson(jsonData);

                    // Also load models from server
                    std::cout << "Loading models for project from server..." << std::endl;
                    proj->LoadModelsFromServer(m_networkClient);

                    // Set as current project
                    currentProject = proj.get();
                    showProjectWindow = false;
                    std::cout << "Project loaded from server: " << currentProject->GetName() << std::endl;

                    // Clear chat history for the new project
                    chatHistory.clear();
                    break;
                }
            }
            });
    }
}

void ClientProjectManager::CreateNewProject(const std::string& name, const std::string& rootDir)
{
    auto newProject = std::make_unique<ClientProject>(m_window, name, rootDir);

    // Generate a unique ID if we'll be saving to server
    if (isLoggedIn && m_networkClient) {
        std::string projectId = GenerateUniqueId();
        newProject->SetProjectId(projectId);
    }

    // Add to projects collection
    m_projects.push_back(std::move(newProject));
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

void ClientProjectManager::SyncWithServer()
{
    if (!isLoggedIn || !m_networkClient || !currentProject) {
        return;
    }

    std::cout << "Syncing with server..." << std::endl;

    // 1. Check if we have local changes to push
    if (currentProject->IsModifiedLocally()) {
        std::string projectId = currentProject->GetProjectId();
        std::string name = currentProject->GetName();
        std::string jsonData = currentProject->SerializeToJson();

        // Send changes to server
        m_networkClient->pushProject(projectId, name, jsonData);

        // Reset modified flag
        currentProject->SetLocallyModified(false);
        std::cout << "Project pushed to server: " << name << std::endl;
    }

    // 2. Check if we have model changes to push
    if (currentProject->AreModelsModified()) {
        // Sync models with server
        if (currentProject->SyncModels(m_networkClient)) {
            currentProject->SetModelsModified(false);
            std::cout << "Models synced with server" << std::endl;
        }
    }

    // 3. Refresh project list to check for any new projects
    LoadProjectsFromServer();
}

void ClientProjectManager::OnModelUpdatedFromServer(const std::string& projectId, const std::string& modelPath)
{
    // Only handle if this is for our current project
    if (!currentProject || currentProject->GetProjectId() != projectId) {
        return;
    }

    std::cout << "Received model update notification for: " << modelPath << std::endl;

    // Check if we need to download the model
    bool needToDownload = true;
    const auto& localModels = currentProject->GetModelPaths();
    auto it = std::find(localModels.begin(), localModels.end(), modelPath);

    if (it != localModels.end()) {
        // Model exists in our list, check if we modified it locally
        if (currentProject->AreModelsModified()) {
            // We have local changes, handle conflict
            // For now, we'll take the server version but this could be more sophisticated
            std::cout << "Potential conflict with local model changes. Taking server version." << std::endl;
        }
    }
    else {
        // This is a new model for us, add it to our list
        currentProject->AddModel(modelPath);
        currentProject->SetModelsModified(false); // Don't mark as locally modified
    }

    if (needToDownload) {
        // Download the updated model
        m_networkClient->downloadModel(projectId, modelPath, [this, projectId, modelPath](const std::vector<char>& data) {
            // Create the local directory structure if needed
            std::string localPath = currentProject->GetRootDirectory() + "/" + modelPath;
            fs::path dirPath = fs::path(localPath).parent_path();
            fs::create_directories(dirPath);

            // Save the model file
            std::ofstream modelFile(localPath, std::ios::binary);
            if (modelFile) {
                modelFile.write(data.data(), data.size());
                modelFile.close();
                std::cout << "Downloaded model from server: " << modelPath << std::endl;
            }
            else {
                std::cerr << "Failed to save downloaded model: " << localPath << std::endl;
            }
            });
    }
}

// Method to handle project updates from server
void ClientProjectManager::OnProjectUpdatedFromServer(const std::string& projectId, const std::string& jsonData)
{
    std::cout << "Received project update from server for project ID: " << projectId << std::endl;

    // Find the project in our collection
    for (auto& proj : m_projects) {
        if (proj->GetProjectId() == projectId) {
            // Only update if this is our current project and we're not already modifying it locally
            if (currentProject == proj.get()) {
                if (!currentProject->IsModifiedLocally()) {
                    proj->UpdateFromJson(jsonData);
                    std::cout << "Project updated from server: " << proj->GetName() << std::endl;

                    // Check for new models we need to download
                    nlohmann::json projJson = nlohmann::json::parse(jsonData);
                    if (projJson.contains("models") && projJson["models"].is_array()) {
                        for (const auto& model : projJson["models"]) {
                            std::string modelPath = model;

                            // Check if we have this model locally
                            std::string localPath = proj->GetRootDirectory() + "/" + modelPath;
                            if (!fs::exists(localPath)) {
                                // Need to download this model
                                m_networkClient->downloadModel(projectId, modelPath,
                                    [this, localPath, modelPath](const std::vector<char>& data) {
                                        // Create directories if needed
                                        fs::path dirPath = fs::path(localPath).parent_path();
                                        fs::create_directories(dirPath);

                                        // Save the model file
                                        std::ofstream modelFile(localPath, std::ios::binary);
                                        if (modelFile) {
                                            modelFile.write(data.data(), data.size());
                                            modelFile.close();
                                            std::cout << "Downloaded model from server: " << modelPath << std::endl;
                                        }
                                        else {
                                            std::cerr << "Failed to save downloaded model: " << localPath << std::endl;
                                        }
                                    });
                            }
                        }
                    }
                }
                else {
                    // We have local changes, handle conflict resolution
                    std::cout << "Project update received but we have local changes. Conflict resolution needed." << std::endl;

                    // Compare timestamps for a basic conflict resolution strategy
                    try {
                        nlohmann::json serverJson = nlohmann::json::parse(jsonData);
                        std::time_t serverTime = serverJson["last_modified"];
                        std::time_t localTime = std::chrono::system_clock::to_time_t(currentProject->GetLastModifiedTime());

                        if (serverTime > localTime) {
                            // Server version is newer, prompt user
                            showConflictDialog = true;
                            serverUpdateData = jsonData;
                        }
                        else {
                            // Our version is newer, push it
                            m_networkClient->pushProject(projectId, currentProject->GetName(), currentProject->SerializeToJson());
                        }
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error handling project conflict: " << e.what() << std::endl;
                    }
                }
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

// Method to record local changes for conflict resolution
void ClientProjectManager::RecordChange(const std::string& operation, const std::string& target)
{
    if (currentProject) {
        ChangeRecord record;
        record.timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        record.operation = operation;
        record.target = target;
        localChanges.push_back(record);

        // Also mark the project as modified
        currentProject->SetLocallyModified(true);
    }
}

// Method to handle conflict resolution UI
void ClientProjectManager::ShowConflictResolutionDialog()
{
    if (!showConflictDialog) return;

    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
    ImGui::Begin("Project Conflict", &showConflictDialog);

    ImGui::TextWrapped("There is a conflict between your local changes and server changes.");
    ImGui::TextWrapped("How would you like to resolve this?");

    ImGui::Separator();

    if (ImGui::Button("Use Server Version (Discard Local Changes)")) {
        // Update from server, discarding local changes
        if (currentProject) {
            currentProject->UpdateFromJson(serverUpdateData);
            currentProject->SetLocallyModified(false);
            localChanges.clear();
        }
        showConflictDialog = false;
    }

    ImGui::SameLine();

    if (ImGui::Button("Keep Local Version (Overwrite Server)")) {
        // Push local changes to server
        if (currentProject && m_networkClient) {
            m_networkClient->pushProject(
                currentProject->GetProjectId(),
                currentProject->GetName(),
                currentProject->SerializeToJson()
            );
        }
        showConflictDialog = false;
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel (Decide Later)")) {
        showConflictDialog = false;
    }

    ImGui::End();
}

void ClientProjectManager::ShowChatWindow() {
   // if (!currentProject) return;

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

    if (ImGui::InputText("##chatInput", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (strlen(buffer) > 0) {
            SendChatMessage(buffer);
            memset(buffer, 0, sizeof(buffer));
            chatMessage.clear();
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Send")) {
        if (!chatMessage.empty()) {
            SendChatMessage(chatMessage);
            chatMessage.clear();
        }
    }

    ImGui::End();
}

void ClientProjectManager::SendChatMessage(const std::string& message) {
    if (m_networkClient && currentProject && isLoggedIn) {
        // Add a prefix to identify this as a chat message for the current project
        std::string formattedMessage = "CHAT_PROJECT " + currentProject->GetProjectId() + " " + message;
        m_networkClient->sendMessageToServer(formattedMessage);

        // Add to local history
        std::string displayMessage = currentUsername + ": " + message;
        chatHistory.push_back(displayMessage);
    }
}