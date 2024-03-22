#include "SampleScene.h"

SampleScene::SampleScene(std::string name,Graphics& g)
	:Scene(name,g),
	m_graphics(g),
	cubeShader(g),
	cube(g,cubeShader),
	camera(nullptr),
	pespectiveCamera(nullptr),
	orthographicCamera(nullptr)
{
	Initialize();
	
}

void SampleScene::Initialize()
{
	pespectiveCamera = new PerspectiveCamera();
	pespectiveCamera->SetCamera(90, m_graphics.getAspectRatio(), 1.0f, 1000.0f);

	orthographicCamera = new OrthographicCamera();
	orthographicCamera->SetCamera(m_graphics.getWidth(), m_graphics.getHeight(), 1.0f, 1000.0f);


	//camera = new SceneCamera(pespectiveCamera);
	camera = new SceneCamera(orthographicCamera);
	camera->SetOrthographicCamera(orthographicCamera);
	
	
	cube.CreateCube();

	cubeShader.LoadShaders(L"Assets/shader/VertexShader.cso",
		L"Assets/shader/PixelShader.cso");

	
}

void SampleScene::Update(float delta)
{
	if (camera->isPerspectiveCamera())
	{
		SwitchToPerspective();
	}
	else
	{
		SwitchToOrthographic();
	}


	//DirectX::XMVECTOR pos = DirectX::XMVectorSet(1.0f, 2.0f* (delta*0.9f), 3.0f, 1.0f); // Initialize to (1, 2, 3, 1) for position
	//camera.GetPerspective().SetPosition(pos);
	//camera->getActiveCamera()->Update(delta);
	camera->getActiveCamera()->Update(delta);
	XMFLOAT3 scale = { 0.0f,0.0f,2.0f * delta };
	cube.setRotation(scale);
	XMFLOAT3 scale1 = { 100.0f,100.0f,1.0f * delta };
	//cube.setScale(scale1);
	cube.Update(delta);

}

void SampleScene::Render() 
{
	m_graphics.SetViewMatrix(camera->getActiveCamera()->GetView());///
	m_graphics.SetProjectionMatrix(camera->getActiveCamera()->GetProjectionMatrix());
	m_graphics.controlWindow();
	camera->ControlWindow();
	cubeShader.BindShaders();
	cube.Render();
}

void SampleScene::SwitchToPerspective()
{
	camera->SetPerspectiveCamera(pespectiveCamera);
}

void SampleScene::SwitchToOrthographic()
{
	camera->SetOrthographicCamera(orthographicCamera);
}
