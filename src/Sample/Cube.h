#pragma once
#include "models\ModelBuilder.h"

class Graphics;
class ShaderManager;
#include "models/Vertex.h"

class Cube 
{

public:
	Cube(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> manager);
	void Update(float deltaTime);
	void Render();


private:
	Graphics& m_graphic;
	ModelBuilder builder;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	std::vector<Vertex> v;
	std::vector<unsigned short> i;

	
	
};

