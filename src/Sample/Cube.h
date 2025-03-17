#pragma once
#include "models\ModelBuilder.h"
#include "Scene/GameObject.h"
#include "Scene/GameObject.h"


class Graphics;
class ShaderManager;
#include "models/Vertex.h"

class Cube :public GameObject
{

public:
	Cube(const std::string& name,Graphics& gfx);
	void Update(Graphics& gfx,float deltaTime);
	void Render(Graphics& gfx);
	void controlWindow(Graphics& gfx);



private:
	ModelBuilder builder;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	
	
};

