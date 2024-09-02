#include "SceneManager.h"
#include "Graphics\Graphics.h"
#include "Scene\Scene.h"
#include "imgui/imgui.h"
#include "Graphics/Utilis.h"
#include "window/Window.h"


SceneManager::SceneManager (Window* win)
    :
    m_graphics(win->GetInstance()),
    activeScene(nullptr),
    thumbnail(nullptr)
{
    activeScene = new Scene("begin", *win);
    AddScene(activeScene);
    showSceneWindow = true; 
    thumbnail = std::make_unique<Utils::Texture>(m_graphics, "Assets/textures/thumbnail/Alchemist.png");
    Logo = std::make_unique<Utils::Texture>(m_graphics, "Assets/textures/thumbnail/Alchemist.png");
    minimize= std::make_unique<Utils::Texture>(m_graphics, "Assets/textures/thumbnail/minimize.png");
    maximize = std::make_unique<Utils::Texture>(m_graphics, "Assets/textures/thumbnail/maximize.png");
    close = std::make_unique<Utils::Texture>(m_graphics, "Assets/textures/thumbnail/close.png");
    serializer = new SceneSerializer(*activeScene,m_graphics);

}

SceneManager::~SceneManager() {
    for (auto scene : scenes)
        delete scene;
}

void SceneManager::AddScene(Scene* scene) {
    scenes.push_back(scene);
}

void SceneManager::RemoveScene(Scene* scene) {
    auto it = std::find(scenes.begin(), scenes.end(), scene);
    if (it != scenes.end()) {
        delete* it;
        scenes.erase(it);
    }
}

void SceneManager::SetActiveScene(int index) {
    if (index >= 0 && index < scenes.size())
        activeScene = scenes[index];
}

void SceneManager::SetActiveScene(const std::string& name) {
    for (auto scene : scenes) {
        if (scene->GetName() == name) {
            activeScene = scene;
            return;
        }
    }
}

void SceneManager::Update(float deltaTime) {
    if (activeScene)
        activeScene->Update(deltaTime);
}

void SceneManager::Render() 
{
    thumbnail->Bind();
    this->ControlWindow();
    if (activeScene)
        activeScene->Render();
}
void SceneManager::ControlWindow()
{ 

    if (ImGui::BeginMainMenuBar())
    {
        ImTextureID tex_id_1 = Logo->GetSRV();
        if (ImGui::ImageButton(tex_id_1, ImVec2(20, 20)))
        {
            
        }
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene"))
            {
                createScenePopup = true;
            }
            if (ImGui::MenuItem("Load Scene"))
            {
                serializer->Deserialize("flavian.json");
            }
            if (ImGui::MenuItem("Save Scene"))
            {
                serializer->Serialize("flavian.json");
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                PostMessage(get<0>(m_graphics.getWin()), WM_CLOSE, 0, 0);

            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
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
            ShowWindow(get<0>(m_graphics.getWin()), SW_MINIMIZE);
        }
        ImGui::SameLine();
        ImTextureID tex_id_3 = maximize->GetSRV();

        if (ImGui::ImageButton(tex_id_3, ImVec2(20, 20)))
        {
            ShowWindow(get<0>(m_graphics.getWin()), IsZoomed(get<0>(m_graphics.getWin())) ? SW_RESTORE : SW_MAXIMIZE);
        }
        ImGui::SameLine();
        ImTextureID tex_id_4 = close->GetSRV();
        if (ImGui::ImageButton(tex_id_4, ImVec2(20, 20)))
        {
            PostMessage(get<0>(m_graphics.getWin()), WM_CLOSE, 0, 0);
        }
        // Handle dragging
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows))
        {
            if (!g_isDragging)
            {
                g_isDragging = true;
                GetCursorPos(&g_dragStartPoint);
                RECT windowRect;
                GetWindowRect(get<0>(m_graphics.getWin()), &windowRect);
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

            SetWindowPos(get<0>(m_graphics.getWin()), NULL, g_windowStartPoint.x + dx, g_windowStartPoint.y + dy, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }


        ImGui::EndMainMenuBar();
    }

    RECT rect;
    GetClientRect(get<0>(m_graphics.getWin()), &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Set the main ImGui window to match the size of the main window
   // ImGui::SetNextWindowPos(ImVec2(500.0f, ImGui::GetFrameHeight()));
    //ImGui::SetNextWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height - ImGui::GetFrameHeight())));

    if (ImGui::Begin("Scenes", &showSceneWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::BeginChild("Scene List", ImVec2(600, 80), true);
        for (const auto& scene : scenes)
        {
            ImGui::PushID(scene->GetName().c_str()); // Ensure unique ID for ImGui elements
            ImGui::BeginGroup(); // Group thumbnail and label together

            ImTextureID tex_id = thumbnail->GetSRV();
            if (ImGui::ImageButton(tex_id, ImVec2(60, 60)))
            {
                activeScene = scene;
            }
            ImGui::Text("%s", scene->GetName().c_str()); // Display scene name below thumbnail
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::PopID();
        }
        ImGui::EndChild();
        ImGui::Separator();
        ImGui::SameLine();

        static char sceneName[128] = "";
        ImGui::InputText("Scene Name", sceneName, IM_ARRAYSIZE(sceneName));
        if (ImGui::Button("Create new scene"))
        {
            if (!std::any_of(scenes.begin(), scenes.end(), [&](Scene* scene)
                {
                    return scene->GetName() == sceneName;
                }))
            {
                //fix the creation of the scene later
             //   Scene* newScene = new Scene(sceneName,win );
              //  AddScene(newScene);
                
            }

        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Scene") && activeScene != nullptr)
        {
            RemoveScene(activeScene);
            activeScene = nullptr;
        }
      
    }
    ImGui::End();
}