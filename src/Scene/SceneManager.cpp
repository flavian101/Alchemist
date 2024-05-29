#include "SceneManager.h"

SceneManager::SceneManager(Graphics& g, Window& win)
    :
    m_graphics(g),
    m_window(win),
    activeScene(nullptr),
    thumbnail(nullptr)
{
    activeScene = new Scene("begin", g, win);
    AddScene(activeScene);
    showSceneWindow = true; 
    thumbnail = new Utils::Texture(g);
    thumbnail->LoadTexture("Assets/textures/thumbnail/thumbnail.png", 1u);
    serializer = new SceneSerializer(*activeScene);

    

}

SceneManager::~SceneManager() {
    delete thumbnail;
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
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Scene"))
        {
            createScenePopup = true;
        }
        if (ImGui::MenuItem("Load Scene"))
        {
          
        }
        if (ImGui::MenuItem("Save Scene"))
        {
            serializer->Serialize("flavian.json");
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))
        {
           
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
       
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if (ImGui::Begin("Scenes", &showSceneWindow, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::BeginChild("Scene List", ImVec2(600, 80), true);
        for (const auto& scene : scenes)
        {
            ImGui::PushID(scene->GetName().c_str()); // Ensure unique ID for ImGui elements
            ImGui::BeginGroup(); // Group thumbnail and label together

            ImTextureID tex_id = thumbnail->GetSRV();
            if (ImGui::ImageButton(tex_id, ImVec2(40, 40)))
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
                Scene* newScene = new Scene(sceneName, m_graphics, m_window);
                AddScene(newScene);
                
            }

        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Scene") && activeScene != nullptr)
        {
            RemoveScene(activeScene);
            activeScene = nullptr;
        }
        ImGui::End();
    }

  
      
          
         
            
      
    
}