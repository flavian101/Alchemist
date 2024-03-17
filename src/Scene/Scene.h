#pragma once
#include "models\Model.h"

class Scene
{
public:
	Scene(const std::string& name, Graphics& g);

	void AddObject(RenderableObject& object);
	void RemoveObject(RenderableObject& object);
	void Update(float time);
	void Render();

	std::string GetName()const { return m_name; }
	




private:
	Graphics& m_graphics;
	std::vector<RenderableObject> m_objects;
	std::string m_name;
};

