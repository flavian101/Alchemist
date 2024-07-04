#include "Scene.h"

Scene::Scene(const std::string& name, Graphics& g, Window& win)
	:
	m_name(name),
	m_graphics(g),
	m_win(win),
	sceneCamera(nullptr),
	cube(nullptr),			
	plane(nullptr),
	m_selectedObject(nullptr),
    light(nullptr)
{

	//initialize shadermanager
    auto defaultShader = std::make_shared<ShaderManager>(m_graphics);
    defaultShader->LoadShaders(L"Assets/shader/VertexShader.hlsl", L"Assets/shader/PixelShader.hlsl");
    defaultShader->SetShaderLayout("POSITION|COLOR");

    auto texturedShader = std::make_shared<ShaderManager>(m_graphics);
    texturedShader->LoadShaders(L"Assets/shader/T_vertexShader.hlsl", L"Assets/shader/T_pixelShader.hlsl");
    texturedShader->SetShaderLayout("POSITION|TEXCOORD|NORMAL|TANGENT");

    shaders.push_back(defaultShader);
    shaders.push_back(texturedShader);

    //shaderEditor
    editor = std::make_unique<ShaderEditor>(texturedShader);

	//cameras
	sceneCamera = new SceneCamera("main",m_graphics);
    //light
    //light = std::make_unique<DirectionalLight>("Directional", m_graphics, texturedShader);
    light = new DirectionalLight("Directional", m_graphics, texturedShader);
    AddRederableObjects(dynamic_cast<Light*>(light));
	//model loading 
    //player
    player = std::make_unique<Player>("player", m_graphics, texturedShader);
    AddRederableObjects(player.get());
	cube = new Cube("cube", m_graphics, texturedShader);
	cube->CreateCube();
    AddRederableObjects(cube);

	plane = new Plane("ground", m_graphics, texturedShader);
	plane->CreatePlane(200.0f,200.0f,30.0f,30.0f);
    AddRederableObjects(plane);


}

Scene::~Scene()
{
	delete sceneCamera;
	delete cube;
	delete plane;
}

void Scene::RemoveRederableObjects(RenderableObject* object)
{
	// Remove the object from the vector
	auto it = std::find(objects.begin(), objects.end(), object);
	if (it != objects.end())
	{
		objects.erase(it);
	}
}

void Scene::AddRederableObjects(RenderableObject* object)
{
    objects.push_back(object);
}

void Scene::Update(float deltaTime)
{
	sceneCamera->Update(deltaTime,*player.get());
    for (const auto& objects : objects)
    {
        objects->Update(deltaTime);
    }

}

void Scene::Render()
{
	this->controlWindow();
    for (const auto& object : objects)
    {
        object->Render();
    }
    sceneCamera->Render();


}
void Scene::SetName(const std::string& name)
{
    m_name = name;
}
void Scene::controlWindow()
{
    ImGui::Begin("Scene Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    // Display a list of models in a child window
    if (ImGui::BeginChild("models", ImVec2(0, 200), true))
    {
        ImGui::Text("               Scene Models");
        for (auto& model : objects)
        {
            // Display model names as selectable items
            if (ImGui::Selectable(model->getName().c_str()))
            {
                // Update the selected model
                m_selectedObject = model;
            }
            // Open a context menu when right-clicked on a model
            if (ImGui::BeginPopupContextItem(std::to_string(reinterpret_cast<std::uintptr_t>(model)).c_str()))
            {

                // Add option to rename the model
                if (ImGui::MenuItem("Rename"))
                {
                    // Set the flag to show the rename input field
                    m_renameObject = true;
                }

                // Add option to delete the model
                if (ImGui::MenuItem("Delete"))
                {
                    // Remove the model from the scene
                    RemoveRederableObjects(model);
                }
                if (ImGui::BeginMenu("Change Shader"))
                {
                    for (size_t i = 0; i < shaders.size(); ++i)
                    {
                        if (ImGui::MenuItem(("Shader " + std::to_string(i)).c_str())) {
                            model->SetShaderManager(shaders[i]);
                        }
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndPopup();
            }
        }
        //scene Cameras
        ImGui::Text("               Scene Cameras: ");
        for (const auto& [name, camera] : sceneCamera->m_cameras)
        {
            bool isSelected = (sceneCamera->m_selectedCamera == camera);
            if (ImGui::Selectable(name.c_str(), isSelected))
            {
                if (isSelected) {
                    sceneCamera->m_selectedCamera = sceneCamera->GetSelectedCamera();
                }
                else {
                    sceneCamera->m_selectedCamera = camera;
                }                
                m_selectedObject = nullptr; // Deselect the model
            }
        }
        ImGui::EndChild();
    }

    static char objectName[128] = "";
    static int objectType = 0; // 0 for Model, 1 for Light

    // Dropdown to select object type
    ImGui::Combo("Object Type", &objectType, "Model\0Directional Light\0Point Light\0spot Light\0");

    // Input text field to name the new object
    ImGui::InputText("New Name", objectName, IM_ARRAYSIZE(objectName));

    // Button to add the object
    if (ImGui::Button("Add Object", ImVec2(100, 0)))
    {
        if (objectName[0] != '\0')
        {
            RenderableObject* newObject = nullptr;

            if (objectType == 0) // Model
            {
                newObject = new Model(objectName, m_graphics, shaders[0]);
                dynamic_cast<Model*>(newObject)->CreateMesh(cube->getMesh()->getVertices(), cube->getMesh()->getIndices());
            }
            else if (objectType == 1) // Light
            {
                newObject = new DirectionalLight(objectName, m_graphics, shaders[0]);
               // AddRederableObjects(dynamic_cast<Light*>(newObject));
                //objects.push_back(dynamic_cast<DirectionalLight*>(newObject));
            }
            else if (objectType == 2) // Light
            {
               // newObject = new DirectionalLight(objectName, m_graphics, shaders[0]);
                // AddRederableObjects(dynamic_cast<Light*>(newObject));
                 //objects.push_back(dynamic_cast<DirectionalLight*>(newObject));
            }
            else if (objectType == 3) // Light
            {
                //newObject = new DirectionalLight(objectName, m_graphics, shaders[0]);
                // AddRederableObjects(dynamic_cast<Light*>(newObject));
                 //objects.push_back(dynamic_cast<DirectionalLight*>(newObject));
            }
            else if (objectType == 4) // Light
            {
                //newObject = new DirectionalLight(objectName, m_graphics, shaders[0]);
                // AddRederableObjects(dynamic_cast<Light*>(newObject));
                 //objects.push_back(dynamic_cast<DirectionalLight*>(newObject));
            }

            if (newObject)
            {
                AddRederableObjects(newObject);
                m_selectedObject = newObject; // Select the new object
                objectName[0] = '\0'; // Clear the name buffer
            }
        }
    }
    // Display properties of the selected scene object
    if (m_selectedObject)
    {
        m_selectedObject->controlWindow();
    }
    else if (sceneCamera->m_selectedCamera)
    {
        sceneCamera->ControlWindow();
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
            if (ImGui::BeginTabItem("General")) {
                m_graphics.ControlWindow();
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Shader Editor")) {
                editor->Render();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
    }
    ImGui::End();
}

