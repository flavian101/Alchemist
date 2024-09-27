#include "Scene.h"
#include "Graphics/Graphics.h"
#include "Scene/GameObject.h"
#include "Sample\Cube.h"
#include "Sample\Plane.h"
#include "Scene\SceneCamera.h"
#include "window\Window.h"
#include "environment/DirectionalLight.h"
#include "Shaders/ShaderEditor.h"
#include "Grid.h"
#include "models/ModelLoader.h"
#include "Player.h"
#include "Scene/Shaders/ShaderManager.h"


Scene::Scene(const std::string& name, Window& win)
	:
	m_name(name),
	sceneCamera(nullptr),
	cube(nullptr),			
	plane(nullptr),
	m_selectedObject(nullptr),
    light(nullptr)
{
    //cameras
    sceneCamera = new SceneCamera("main", win.GetInstance());

	//initialize shadermanager
    auto defaultShader = std::make_shared<ShaderManager>(win.GetInstance(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"Assets/shader/VertexShader.hlsl", L"Assets/shader/PixelShader.hlsl");
    defaultShader->SetShaderLayout(win.GetInstance(),"POSITION|COLOR");

    auto texturedShader = std::make_shared<ShaderManager>(win.GetInstance(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, L"Assets/shader/T_vertexShader.hlsl", L"Assets/shader/T_pixelShader.hlsl");
    texturedShader->SetShaderLayout(win.GetInstance(),"POSITION|TEXCOORD|NORMAL|TANGENT");

    auto gridShader = std::make_shared<ShaderManager>(win.GetInstance(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST, L"Assets/shader/GridVertex.hlsl", L"Assets/shader/GridPixel.hlsl");
    gridShader->SetShaderLayout(win.GetInstance(),"POSITION|COLOR");
    
    shaders.emplace_back(defaultShader);
    shaders.emplace_back(gridShader);
    shaders.emplace_back(texturedShader);

    //shaderEditor
    editor = std::make_unique<ShaderEditor>(texturedShader);

	
    //light
    //light = std::make_unique<DirectionalLight>("Directional", m_graphics, texturedShader);
    light = new DirectionalLight("Directional", win.GetInstance(), texturedShader);
   // AddGameObject(light);
	//model loading 
    grid = std::make_unique<Grid>("grid", win.GetInstance(), gridShader);
    //player
    player = std::make_unique<Player>("player", win.GetInstance(), texturedShader);


	cube = new Cube("cube", win.GetInstance(),texturedShader);

	plane = new Plane("ground", texturedShader);
	plane->CreatePlane(win.GetInstance(),200.0f,200.0f,30.0f,30.0f);

    //m_model = new ModelLoader("Assets/model/ghost/ghost.glb",m_graphics, texturedShader);
    //m_model = new ModelLoader("Assets/model/gobber/GoblinX.obj",win.GetInstance(), texturedShader);
    m_model = new ModelLoader("Assets/model/ring.gltf", win.GetInstance(), texturedShader);
    //m_model = new ModelLoader("Assets/model/boxy.gltf",win.GetInstance(), texturedShader);
     // m_model = new ModelLoader("Assets/model/nano.gltf",m_graphics, texturedShader);
   // m_model = new ModelLoader("Assets/model/muro/muro.obj",win.GetInstance(), texturedShader);
     //m_model = new ModelLoader("Assets/model/nano_textured/nanosuit.obj",m_graphics, texturedShader);
     //m_model = new ModelLoader("Assets/model/nano_hierarchy.gltf", win.GetInstance(), texturedShader);
   // m_model->LoadModel("Assets/model/nano.gltf");

}

Scene::~Scene()
{
	delete sceneCamera;
	delete cube;
	delete plane;
    delete light;
    shaders.clear();

}

void Scene::RemoveGameObject(GameObject* object)
{
	// Remove the object from the vector
	auto it = std::find(objects.begin(), objects.end(), object);
	if (it != objects.end())
	{
		objects.erase(it);
	}
}

void Scene::AddGameObject(GameObject* object)
{
    objects.emplace_back(object);
}

void Scene::Update(Graphics& gfx,float deltaTime)
{
	sceneCamera->Update(deltaTime,*player.get());
    light->Update(gfx, deltaTime);
    m_model->Update(gfx,deltaTime);

   // player->Update(deltaTime);
   // cube->Update(deltaTime);
   // plane->Update(deltaTime);
    for (const auto& objects : objects)
    {
        objects->Update(gfx,deltaTime);
    }

}

void Scene::Render(Graphics& gfx)
{
    shaders[2]->BindShaders(gfx);
    light->Render(gfx);
    m_model->Render(gfx);
    m_model->controlWindow();
	this->controlWindow(gfx);
    for (const auto& object : objects)
    {
        object->Render(gfx);
    }
    sceneCamera->Render(gfx);
    m_model->controlWindow();
   // player->Render();
   // cube->Render();
   // plane->Render();


}
void Scene::SetName(const std::string& name)
{
    m_name = name;
}
void Scene::controlWindow(Graphics& gfx)
{
    ImGui::Begin("Scene Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
    light->controlWindow();

    // Detect right-click in the empty space to open the Add Object dialog
    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("Add Object Context Menu");
    }

    // Add Object Context Menu
    if (ImGui::BeginPopup("Add Object Context Menu"))
    {
        if (ImGui::MenuItem("Add Object"))
        {
            ImGui::OpenPopup("Add Object");
        }
        ImGui::EndPopup();
    }

    // Add Object Dialog
    if (ImGui::BeginPopupModal("Add Object", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char objectName[128] = "";
        static int objectType = 0; // 0 for Model, 1 for Light

        ImGui::Combo("Object Type", &objectType, "Model\0Directional Light\0Point Light\0Spot Light\0");
        ImGui::InputText("New Name", objectName, IM_ARRAYSIZE(objectName));

        if (ImGui::Button("Add", ImVec2(100, 0)))
        {
            if (objectName[0] != '\0')
            {
                GameObject* newObject = nullptr;

                if (objectType == 0) // Model
                {
                    //newObject = new Model(objectName, shaders[0]);
                   // dynamic_cast<Model*>(newObject)->CreateMesh(cube->getVertices(), cube->getMesh()->getIndices());
                }
                else if (objectType == 1) // Directional Light
                {
                    //newObject = new DirectionalLight(objectName, gfx, shaders[0]);
                }
                else if (objectType == 2) // Point Light
                {
                    // newObject = new PointLight(objectName, m_graphics, shaders[0]);
                }
                else if (objectType == 3) // Spot Light
                {
                    // newObject = new SpotLight(objectName, m_graphics, shaders[0]);
                }

                if (newObject)
                {
                   // AddGameObject(newObject);
                  //  m_selectedObject = newObject; // Select the new object
                    objectName[0] = '\0'; // Clear the name buffer
                }
            }
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(100, 0)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Display a list of models in a child window
    if (ImGui::BeginChild("models", ImVec2(0, 200), true))
    {
        ImGui::Text("Scene Models");
        ImGui::Separator();

        for (auto& model : objects)
        {
            // Display model names as selectable items
           // if (ImGui::Selectable(model->getName().c_str(), m_selectedObject == model))
           // {
           //     // Update the selected model
           //    // m_selectedObject = model;
           // }
            // Open a context menu when right-clicked on a model
           // if (ImGui::BeginPopupContextItem(std::to_string(reinterpret_cast<std::uintptr_t>(model)).c_str()))
           // {
           //     // Add option to rename the model
           //     if (ImGui::MenuItem("Rename"))
           //     {
           //         // Set the flag to show the rename input field
           //         m_renameObject = true;
           //     }
           //
           //     // Add option to delete the model
           //     if (ImGui::MenuItem("Delete"))
           //     {
           //         // Remove the model from the scene
           //       //  RemoveGameObject(model);
           //     }
           //
           //     if (ImGui::BeginMenu("Change Shader"))
           //     {
           //         for (size_t i = 0; i < shaders.size(); ++i)
           //         {
           //             if (ImGui::MenuItem(("Shader " + std::to_string(i)).c_str()))
           //             {
           //                // model->SetShaderManager(shaders[i]);
           //             }
           //         }
           //         ImGui::EndMenu();
           //     }
           //     ImGui::EndPopup();
           // }
        }

        ImGui::Text("Scene Cameras");
        ImGui::Separator();

        for (const auto& [name, camera] : sceneCamera->m_cameras)
        {
            bool isSelected = (sceneCamera->m_selectedCamera == camera);
            if (ImGui::Selectable(name.c_str(), isSelected))
            {
                sceneCamera->m_selectedCamera = camera;
                m_selectedObject = nullptr; // Deselect the model
            }
        }
        ImGui::EndChild();
    }

    ImGui::Separator();

    // Display properties of the selected scene object
    if (m_selectedObject)
    {
        m_selectedObject->controlWindow();
    }
    else if (sceneCamera->m_selectedCamera)
    {
        sceneCamera->ControlWindow(gfx);
    }

    // Show input field for renaming the model
    if (m_renameObject)
    {
        ImGui::OpenPopup("Rename Model");
        if (ImGui::BeginPopup("Rename Model", NULL))
        {
            static char newName[128] = ""; // Buffer to store new name
            ImGui::InputText("New Name", newName, IM_ARRAYSIZE(newName));

            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                // Set the new name for the selected model
                m_selectedObject->setName(newName);
                ImGui::CloseCurrentPopup();
                m_renameObject = false; // Reset the flag
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
                m_renameObject = false; // Reset the flag
            }

            ImGui::EndPopup();
        }
    }

    ImGui::End();

    if (ImGui::Begin("Graphics Settings", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
        ImGui::BeginTabBar("Settings");
        if (ImGui::BeginTabItem("General"))
        {
            gfx.ControlWindow();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Shader Editor"))
        {
            editor->Render(gfx);
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}


void Scene::AddObjectWindow()
{
   
}



