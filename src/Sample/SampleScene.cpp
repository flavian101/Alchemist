#include "SampleScene.h"

SampleScene::SampleScene(std::string name,Graphics& g, Window& win)
	:Scene(name,g),
	m_graphics(g),
	cubeShader(g),
	cube(g,cubeShader),
	plane(g,cubeShader),
	camera(nullptr),
	pespectiveCamera(nullptr),
	orthographicCamera(nullptr),
	input(win),
	selectedCamera(nullptr)
{
	Initialize();
	
}

void SampleScene::Initialize()
{
	cubeShader.LoadShaders(L"Assets/shader/VertexShader.cso",
		L"Assets/shader/PixelShader.cso");

	pespectiveCamera = new PerspectiveCamera();
	pespectiveCamera->SetCamera(45.0f, m_graphics.getAspectRatio(), 1.0f, 1000.0f);
	

	orthographicCamera = new OrthographicCamera();
	orthographicCamera->SetCamera(m_graphics.getWidth(), m_graphics.getHeight(), 1.0f, 5.0f);


	camera = new SceneCamera("main", pespectiveCamera);
	camera->SetPerspectiveCamera(pespectiveCamera);
	
	cube.CreateCube();
	
	

	plane.CreatePlane();
	DirectX::XMFLOAT3 scalep(500.0f, 0.0f, 500.0f);
	plane.setScale(scalep);

	


	
}

void SampleScene::Update(float delta)
{

	// Update selected camera based on user input
	selectedCamera = camera->GetSelectedCamera();

	if (selectedCamera)
	{
		if (selectedCamera->isPerspectiveCamera())
		{
			SwitchToPerspective();
			input.DetectInput(delta, *selectedCamera->GetPerspective(),cube);
		}
		else
		{
			SwitchToOrthographic();
			// Handle input for orthographic camera if needed
		}
	}
	camera->getActiveCamera()->Update(delta);
	cube.Update(delta);
	plane.Update(delta);

}

void SampleScene::Render() 
{
	m_graphics.controlWindow();
	camera->ControlWindow();

	selectedCamera = camera->GetSelectedCamera();
	if (selectedCamera)
	{
		m_graphics.SetViewMatrix(selectedCamera->getActiveCamera()->GetView());///
		m_graphics.SetProjectionMatrix(selectedCamera->getActiveCamera()->GetProjectionMatrix());
	}
	cubeShader.BindShaders();
	cube.Render();
	plane.Render();
}

void SampleScene::SwitchToPerspective()
{
	camera->SetPerspectiveCamera(pespectiveCamera);
}

void SampleScene::SwitchToOrthographic()
{
	camera->SetOrthographicCamera(orthographicCamera);
}
