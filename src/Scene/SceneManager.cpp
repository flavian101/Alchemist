#include "SceneManager.h"
#include "Graphics\Graphics.h"
#include "Scene\Scene.h"
#include "imgui/imgui.h"
#include "Graphics/Utilis.h"
#include "window/Window.h"
#include <fstream>



SceneManager::SceneManager (Window* win)
    :
    m_window(*win),
    activeScene(nullptr)
{
    activeScene = new Scene("begin", *win);
    AddScene(activeScene);
    showSceneWindow = true; 
    thumbnail = std::make_unique<Utils::Texture>(win->GetInstance(), "Assets/textures/thumbnail/Alchemist.png");
    serializer = new SceneSerializer(win->GetInstance());

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

void SceneManager::Update(Graphics& gfx,float deltaTime) {
    if (activeScene)
        activeScene->Update(gfx,deltaTime);
}

void SceneManager::Render(Graphics& gfx)
{
    thumbnail->Bind(gfx);
    this->ControlWindow(gfx);
    if (activeScene)
        activeScene->Render(gfx);
}
void SceneManager::ControlWindow(Graphics& gfx)
{ 

    RECT rect;
    GetClientRect(get<0>(gfx.getWin()), &rect);
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


nlohmann::json SceneManager::SerializeSceneManager(const std::string& projectName,const std::string& projectDir) {
    nlohmann::json j;
    for (const auto& scene : scenes) {
        j["scenes"].push_back(serializer->Serialize(scene, projectDir,projectName));
    }
    return j;
}

void SceneManager::DeserializeSceneManager(const nlohmann::json& j) {

	for (auto& scene : scenes) {
		delete scene; // Clean up existing scenes
	}
	scenes.clear(); // Clear the vector     
        
    for (const auto& sceneJson : j["scenes"]) {
        auto scene = new Scene(sceneJson["Scene Name"], m_window);
        serializer->Deserialize(scene, sceneJson);
        if(!activeScene)
			activeScene = scene;    
        AddScene(scene);
    }
}