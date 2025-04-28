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
class GameObject;



class Scene
{
public:
	Scene(const std::string& name,Window& win);
	~Scene();

	void RemoveGameObject(GameObject* object);
	void AddGameObject(GameObject* object);
	void Update(Graphics& gfx,float time);
	void Render(Graphics& gfx);
	std::string GetName()const { return m_name; }
	std::vector<GameObject*> GetObjects()const { return objects; }
	void SetName(const std::string& name);
	void controlWindow(Graphics& gfx);
	void AddObjectWindow();
	
private:



private:
	friend class SceneSerializer;
	std::string m_name;
	SceneCamera* sceneCamera;
	//std::unique_ptr<ShaderEditor> editor;
	bool m_renameObject = false;
	DirectionalLight* light;

	/// rederables
	std::vector<GameObject*> objects;
	GameObject* m_selectedObject;
	std::unique_ptr<GameObject> newobject;
	ModelLoader* m_model;
};

