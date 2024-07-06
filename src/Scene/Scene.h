#pragma once
#include "Sample\Cube.h"
#include "Sample\Plane.h"
#include "Scene\SceneCamera.h"
#include "window\Window.h"
#include <memory>
#include "environment/DirectionalLight.h"
#include "Shaders/ShaderEditor.h"
#include <map>
#include "Grid.h"
#include "models/ModelLoader.h"

class Scene
{
public:
	Scene(const std::string& name, Graphics& g, Window& win);
	~Scene();

	void RemoveRederableObjects(RenderableObject* object);
	void AddRederableObjects(RenderableObject* object);
	void Update(float time);
	void Render();
	std::string GetName()const { return m_name; }
	void SetName(const std::string& name);
	void controlWindow();
	
private:



private:
	Graphics& m_graphics;
	Window& m_win;
	std::string m_name;
	SceneCamera* sceneCamera;
	std::unique_ptr<Player> player;
	std::unique_ptr<Grid> grid;
	Cube* cube;
	Plane* plane;
	std::vector< std::shared_ptr<ShaderManager>> shaders;
	std::unique_ptr<ShaderEditor> editor;
	bool m_renameObject = false;
	DirectionalLight* light;
	friend class SceneSerializer;

	/// rederables
	std::vector<RenderableObject*> objects;
	RenderableObject* m_selectedObject;
	std::unique_ptr<RenderableObject> newobject;
	ModelLoader* m_model;
};

