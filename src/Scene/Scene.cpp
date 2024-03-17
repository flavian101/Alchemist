#include "Scene.h"

Scene::Scene(const std::string& name, Graphics& g)
	:
	m_graphics(g),
	m_name(name)
	//m_objects(g)
{
	// Initialize scene and renderable objects
}

void Scene::AddObject(RenderableObject& object)
{
	m_objects.push_back(object);
}

void Scene::RemoveObject(RenderableObject& object)
{
	//m_objects.p
}

void Scene::Update(float time)
{
	for (auto obj : m_objects)
	{
	obj.Update(time);

	}
}

void Scene::Render()
{
	for (auto obj : m_objects)
	{
		obj.Render();
	}
}


