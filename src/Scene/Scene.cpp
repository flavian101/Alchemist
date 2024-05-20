#include "Scene.h"

Scene::Scene(const std::string& name, Graphics& g, Window& win)
	:
	m_name(name),
	m_graphics(g),
	m_win(win),
	model(nullptr),
	sceneCamera(nullptr),
	input(nullptr),
	defaultShader(nullptr),
	texturedShader(nullptr),
	cube(nullptr),			
	plane(nullptr),
	m_selectedModel(nullptr)

{
	input = new Input(win);
	//initialize shadermanager
	defaultShader = new ShaderManager(m_graphics);
	defaultShader->LoadShaders(L"Assets/shader/VertexShader.cso",
		L"Assets/shader/PixelShader.cso");
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};
	UINT numElements = ARRAYSIZE(layout);
	defaultShader->SetShaderLayout(layout,numElements);

	texturedShader = new ShaderManager(m_graphics);
	texturedShader->LoadShaders(L"Assets/shader/T_vertexShader.cso",
		L"Assets/shader/T_pixelShader.cso");
	D3D11_INPUT_ELEMENT_DESC TextureLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};
	UINT t_numElements = ARRAYSIZE(TextureLayout);
	texturedShader->SetShaderLayout(TextureLayout, t_numElements);

	
	//cameras
	sceneCamera = new SceneCamera("main",m_graphics,true                   );


	//model loading 
	cube = new Cube("player", m_graphics, *texturedShader);
	cube->CreateCube();
	AddObject(cube);

	plane = new Plane("ground", m_graphics, *defaultShader);
	plane->CreatePlane(200.0f,200.0f,30.0f,30.0f);
	AddObject(plane);

}

Scene::~Scene()
{
	delete sceneCamera;
	delete defaultShader;
	delete texturedShader;
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
	sceneCamera->Update(deltaTime);

    
	input->DetectInput(deltaTime, *sceneCamera->GetSelectedCamera()->GetPerspective(), *cube);

	for (auto obj : m_models)
	{
		obj->Update(deltaTime);

	}
}

void Scene::Render()
{
	m_graphics.controlWindow();
	this->controlWindow();

	sceneCamera->Render();
	for (auto obj : m_models)
	{
		obj->Render();
	}

}
void Scene::controlWindow()
{
    ImGui::Begin("Scene Hierarchy",nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

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
                sceneCamera->m_selectedCamera = isSelected ? nullptr : camera;
                m_selectedModel = nullptr; // Deselect the model
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
            model = new Model(modelName, m_graphics, *defaultShader);

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
}


