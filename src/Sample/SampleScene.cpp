#include "SampleScene.h"

SampleScene::SampleScene(std::string name,Graphics& g)
	:Scene(name,g),
	m_graphics(g),
	cubeShader(g),
	cube(g,cubeShader)
{
	Initialize();
}

void SampleScene::Initialize()
{
	auto cam = camera.CreatePerspective(60);
	cam.SetCamera(60, 9 / 16, 0.1f, 1000.0f);
	m_graphics.SetViewMatrix(cam.GetView());
	m_graphics.SetProjectionMatrix(cam.GetProjectionMatrix());
	
	cubeShader.LoadShaders(L"Assets/shader/VertexShader.cso",
		L"Assets/shader/PixelShader.cso");
	
}

void SampleScene::Update(float delta)
{
	cube.Update(delta);
}

void SampleScene::Render() 
{
	cubeShader.BindShaders();
	cube.Render();
}
