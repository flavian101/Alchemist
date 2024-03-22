#pragma once
#include "Scene\Scene.h"
#include "Cube.h"
#include "Scene\SceneCamera.h"

class SampleScene: public Scene
{
public:
	SampleScene(std::string name, Graphics& g);
	void Initialize();

	void Update(float delta);
	void Render();
	void SwitchToPerspective();
	void SwitchToOrthographic();

private:
	SceneCamera* camera;
	PerspectiveCamera* pespectiveCamera;
	OrthographicCamera* orthographicCamera;
	Graphics& m_graphics;
	ShaderManager cubeShader;
	Cube cube;
	
	
};

