#include "Scene.h"

Scene::Scene(const std::string& name, Graphics& g, Window& win)
	:
	m_name(name),
	m_graphics(g),
	m_win(win),
	model(nullptr),
	sceneCamera(nullptr),
	cube(nullptr),			
	plane(nullptr),
	m_selectedModel(nullptr),
    light(nullptr)
{

	//initialize shadermanager
    auto defaultShader = std::make_shared<ShaderManager>(m_graphics);
    defaultShader->LoadShaders(L"Assets/shader/VertexShader.hlsl", L"Assets/shader/PixelShader.hlsl");
    defaultShader->SetShaderLayout("POSITION|COLOR");

    auto texturedShader = std::make_shared<ShaderManager>(m_graphics);
    texturedShader->LoadShaders(L"Assets/shader/T_vertexShader.hlsl", L"Assets/shader/T_pixelShader.hlsl");
    texturedShader->SetShaderLayout("POSITION|TEXCOORD|NORMAL");

    shaders.push_back(defaultShader);
    shaders.push_back(texturedShader);

    //shaderEditor
    editor = std::make_unique<ShaderEditor>(texturedShader);

	//cameras
	sceneCamera = new SceneCamera("main",m_graphics);
    //light
    light = new EnvironmentLight("main1", m_graphics, texturedShader);

	//model loading 
    //player
    player = std::make_unique<Player>("player", m_graphics, texturedShader);
    AddObject(player.get());
	cube = new Cube("cube", m_graphics, texturedShader);
	cube->CreateCube();
	AddObject(cube);

	plane = new Plane("ground", m_graphics, texturedShader);
	plane->CreatePlane(200.0f,200.0f,30.0f,30.0f);
	AddObject(plane);


}

Scene::~Scene()
{
	delete sceneCamera;
	delete cube;
	delete plane;
}



void Scene::AddObject(Model* object)
{
	m_models.push_back(object);
}

void Scene::RemoveObject(Model* object)
{
	// Remove the object from the vector
	auto it = std::find(m_models.begin(), m_models.end(), object);
	if (it != m_models.end())
	{
		m_models.erase(it);
	}
}

void Scene::Update(float deltaTime)
{
	sceneCamera->Update(deltaTime,*player.get());
    light->Update(deltaTime);
	for (auto obj : m_models)
	{
		obj->Update(deltaTime);

	}
}

void Scene::Render()
{
	this->controlWindow();
    light->Render();
	sceneCamera->Render();
	for (auto obj : m_models)
	{
		obj->Render();
	}

}
void Scene::SetName(const std::string& name)
{
    m_name = name;
}
void Scene::controlWindow()
{
    ImGui::Begin("Scene Hierarchy", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Display a list of models in a child window
    if (ImGui::BeginChild("models", ImVec2(0, 200), true))
    {
        ImGui::Text("Scene Models");
        for (auto& model : m_models)
        {
            // Display model names as selectable items
            if (ImGui::Selectable(model->getName().c_str()))
            {
                // Update the selected model
                m_selectedModel = model;
            }
            // Open a context menu when right-clicked on a model
            if (ImGui::BeginPopupContextItem(std::to_string(reinterpret_cast<std::uintptr_t>(model)).c_str()))
            {

                // Add option to rename the model
                if (ImGui::MenuItem("Rename"))
                {
                    // Set the flag to show the rename input field
                    m_renameModel = true;
                }

                // Add option to delete the model
                if (ImGui::MenuItem("Delete"))
                {
                    // Remove the model from the scene
                    RemoveObject(model);
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
        ImGui::Text("Scene Cameras: ");
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
                }                m_selectedModel = nullptr; // Deselect the model
            }
        }
        ImGui::EndChild();
    }

    static char modelName[128] = ""; // Buffer to store new name
    // Button to add the object
    if (ImGui::Button("Add object", ImVec2(100, 0)))
    {
        // Only create and add the model if modelName is not empty
        if (modelName[0] != '\0')
        {
            model = new Model(modelName, m_graphics, shaders[0]);

            model->CreateMesh(cube->getMesh()->getVertices(), cube->getMesh()->getIndices());

            m_models.push_back(model);

            // Optionally, clear the modelName buffer after adding the model
            modelName[0] = '\0';
        }
    }
    // Input text field to name the new model
    ImGui::InputText("New Name", modelName, IM_ARRAYSIZE(modelName));

    // Display properties of the selected model
    if (m_selectedModel)
    {
        m_selectedModel->controlWindow();
    }
    else if (sceneCamera->m_selectedCamera)
    {
        sceneCamera->ControlWindow();
    }

    // Show input field for renaming the model
    if (m_renameModel)
    {
        ImGui::OpenPopup("Rename Model");
        if (ImGui::BeginPopup("Rename Model", NULL))
        {
            static char newName[128] = ""; // Buffer to store new name
            ImGui::InputText("New Name", newName, IM_ARRAYSIZE(newName));

            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                // Set the new name for the selected model
                m_selectedModel->setName(newName);
                ImGui::CloseCurrentPopup();
                m_renameModel = false; // Reset the flag
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
                m_renameModel = false; // Reset the flag
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

