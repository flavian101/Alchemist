#pragma once
#include "Sample\Cube.h"
#include "Sample\Plane.h"
#include "Scene\SceneCamera.h"
#include "window\Window.h"
#include <memory>
#include "environment/Light.h"
#include "Shaders/ShaderEditor.h"
#include <map>

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
	void SetName(const std::string& name);


	void controlWindow();
	
private:



private:
	Graphics& m_graphics;
	Window& m_win;
	std::vector<Model*> m_models; // Use std::vector<std::unique_ptr<Model>> for ownership
	Model* model;
	std::string m_name;
	SceneCamera* sceneCamera;
	std::unique_ptr<Player> player;
	Cube* cube;
	Plane* plane;
	std::vector< std::shared_ptr<ShaderManager>> shaders;
	std::unique_ptr<ShaderEditor> editor;
	Model* m_selectedModel;
	bool m_renameModel = false;
	EnvironmentLight* light;
	friend class SceneSerializer;

};

