#pragma once
#include "models/Model.h"
#include "models/ModelBuilder.h"
#include "Scene/GameObject.h"


class Graphics;
class ShaderManager;
class Vertex;

class Grid :public GameObject
{
public:
	Grid(const std::string& name, Graphics& gfx);
	void Update(Graphics& gfx, float deltaTime);
	void Render(Graphics& gfx);

	void controlWindow(Graphics& gfx);
private:
	ModelBuilder builder;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

};

