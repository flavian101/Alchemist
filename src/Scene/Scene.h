#pragma once
#include "Sample\Cube.h"
#include "Sample\Plane.h"
#include "Scene\SceneCamera.h"
#include "window\Window.h"
#include "Input\Input.h"
#include "models\Model.h"

class Scene
{
public:
	Scene(const std::string& name, Graphics& g);
	void Create();
	void AddObject(RenderableObject* object);
	void RemoveObject(RenderableObject* object);

	void Update(float time);
	void Render();

	std::string GetName()const { return m_name; }
	




private:
	Graphics& m_graphics;
	std::vector<RenderableObject>* m_objects;
	std::string m_name;
};

