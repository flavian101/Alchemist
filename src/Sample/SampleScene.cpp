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
	input(win)
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


	camera = new SceneCamera(pespectiveCamera);
	camera->SetPerspectiveCamera(pespectiveCamera);
	
	
	cube.CreateCube();
	DirectX::XMFLOAT3 scale(2.0f, 2.0f, 2.0f);
	DirectX::XMFLOAT3 posc(0.0f, 1.0f, 0.0f);

	cube.setScale(scale);
	cube.setTranslation(posc);
	

	plane.CreatePlane();
	DirectX::XMFLOAT3 scalep(500.0f, 0.0f, 500.0f);
	plane.setScale(scalep);

	DirectX::XMFLOAT3 pos(0.0f, 0.0f, 0.0f);
	plane.setTranslation(pos);

	


	
}

void SampleScene::Update(float delta)
{
	if (camera->isPerspectiveCamera())
	{
		SwitchToPerspective();
		input.DetectInput(delta, *pespectiveCamera);

	}
	else
	{
		SwitchToOrthographic();
	}
	camera->getActiveCamera()->Update(delta);
	cube.Update(delta);
	plane.Update(delta);

}

void SampleScene::Render() 
{
	m_graphics.SetViewMatrix(camera->getActiveCamera()->GetView());///
	m_graphics.SetProjectionMatrix(camera->getActiveCamera()->GetProjectionMatrix());
	m_graphics.controlWindow();
	camera->ControlWindow();
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
