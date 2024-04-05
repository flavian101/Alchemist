#include "Scene.h"

Scene::Scene(const std::string& name, Graphics& g, Window& win)
	:
	m_graphics(g),
	m_name(name),
	sceneCamera(nullptr)	,
	input(nullptr),
	defaultShader(nullptr),			
	perspectiveCamera(nullptr),	
	orthographicCamera(nullptr), 
	cube(nullptr),			
	plane(nullptr)			

{
	input = new Input(win);
	//initialize shadermanager
	defaultShader = new ShaderManager(m_graphics);
	defaultShader->LoadShaders(L"Assets/shader/VertexShader.cso",
		L"Assets/shader/PixelShader.cso");

	perspectiveCamera = new PerspectiveCamera();
	perspectiveCamera->SetCamera(45.0f, m_graphics.getAspectRatio(), 1.0f, 1000.0f);


	orthographicCamera = new OrthographicCamera();
	orthographicCamera->SetCamera(m_graphics.getWidth(), m_graphics.getHeight(), 1.0f, 5.0f);
	

	sceneCamera = new SceneCamera("main", perspectiveCamera);
	sceneCamera->SetPerspectiveCamera(perspectiveCamera);

	//model loading 
	cube = new Cube(m_graphics, *defaultShader);
	cube->CreateCube();
	AddObject(cube);

	plane = new Plane(m_graphics, *defaultShader);
	plane->CreatePlane();
	AddObject(plane);





	// Initialize scene and renderable objects
}

Scene::~Scene()
{
	delete sceneCamera;
	//delete pespectiveCamera;
	//delete orthographicCamera;
	//delete controller;
	delete defaultShader;
	delete cube;
	delete plane;
}



void Scene::AddObject(Model* object)
{
	m_models.push_back(object);
}

void Scene::RemoveObject(Model* object)
{
	//m_objects.p
}

void Scene::Update(float deltaTime)
{
	selectedCamera = sceneCamera->GetSelectedCamera();
	if (selectedCamera)
	{
		if (selectedCamera->isPerspectiveCamera())
		{
			SwitchToPerspective();
			input->DetectInput(deltaTime, *selectedCamera->GetPerspective(), *cube);
		}
		else
		{
			SwitchToOrthographic();
		}

	}
	sceneCamera->getActiveCamera()->Update(deltaTime);
	for (auto obj : m_models)
	{
		obj->Update(deltaTime);

	}
}

void Scene::Render()
{
	m_graphics.controlWindow();
	sceneCamera->ControlWindow();

	selectedCamera = sceneCamera->GetSelectedCamera();
	if (selectedCamera)
	{
		m_graphics.SetViewMatrix(selectedCamera->getActiveCamera()->GetView());
		m_graphics.SetProjectionMatrix(selectedCamera->getActiveCamera()->GetProjectionMatrix());
	}
	defaultShader->BindShaders();

	for (auto obj : m_models)
	{
		obj->Render();
	}
}

void Scene::SwitchToPerspective()
{
	sceneCamera->SetPerspectiveCamera(perspectiveCamera);
}

void Scene::SwitchToOrthographic()
{
	sceneCamera->SetOrthographicCamera(orthographicCamera);
}

void Scene::controlWindow()
{
	if (ImGui::Begin("Scene objects"))
	{



	}
	ImGui::End();
}


