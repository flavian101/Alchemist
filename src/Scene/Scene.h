#pragma once
#include "Sample\Cube.h"
#include "Sample\Plane.h"
#include "Scene\SceneCamera.h"
#include "window\Window.h"
#include "Input\Input.h"
#include "models\Model.h"
#include <memory>

class Scene
{
public:
	Scene(const std::string& name, Graphics& g, Window& win);
	~Scene();

	void AddObject(Model* object);

	void RemoveObject(Model* object);

	void Update(float time);
	void Render();

	void SwitchToPerspective();

	void SwitchToOrthographic();

	std::string GetName()const { return m_name; }

	void controlWindow();
	




private:
	Graphics& m_graphics;
	std::vector<Model*> m_models; // Use std::vector<std::unique_ptr<Model>> for ownership
	std::string m_name;
	CameraController* controller;
	SceneCamera* selectedCamera;
	SceneCamera* sceneCamera;
	PerspectiveCamera* perspectiveCamera;
	OrthographicCamera* orthographicCamera;
	Cube* cube;
	Plane* plane;
	Input* input;
	ShaderManager* defaultShader;
	Model* m_selectedModel;
	bool m_renameModel = false;

};

