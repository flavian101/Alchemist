#pragma once
#include "Sample\Cube.h"
#include "Sample\Plane.h"
#include "Scene\SceneCamera.h"
#include "window\Window.h"
#include "Input\Input.h"
#include "models\Model.h"
#include <memory>
#include "environment/Light.h"

class Scene
{
public:
	Scene(const std::string& name, Graphics& g, Window& win);
	~Scene();

	void AddObject(Model* object);

	void RemoveObject(Model* object);

	void Update(float time);
	void Render();

	std::string GetName()const { return m_name; }

	void controlWindow();
	




private:
	Graphics& m_graphics;
	Window& m_win;
	std::vector<Model*> m_models; // Use std::vector<std::unique_ptr<Model>> for ownership
	Model* model;
	std::string m_name;
	SceneCamera* sceneCamera;
	Cube* cube;
	Plane* plane;
	Input* input;
	std::shared_ptr<ShaderManager> defaultShader;
	std::shared_ptr<ShaderManager> texturedShader;
	Model* m_selectedModel;
	bool m_renameModel = false;
	EnvironmentLight* light;
};

