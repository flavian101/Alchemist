#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>

class Graphics;
class ModelLoader;
class Grid;
class ShaderEditor;
class ShaderManager;
class Window;
class SceneCamera;
class Plane;
class Cube;
class DirectionalLight;
class Player;
class RenderableObject;



class Scene
{
public:
	Scene(const std::string& name, Graphics& g, Window& win);
	~Scene();

	void RemoveRenderableObject(RenderableObject* object);
	void AddRenderableObject(RenderableObject* object);
	void Update(float time);
	void Render();
	std::string GetName()const { return m_name; }
	void SetName(const std::string& name);
	void controlWindow();
	void AddObjectWindow();
	
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

