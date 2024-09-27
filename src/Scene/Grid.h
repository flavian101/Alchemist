#pragma once
#include "models/Model.h"
#include "models/ModelBuilder.h"

class Graphics;
class ShaderManager;
class Vertex;

class Grid
{
public:
	Grid(const std::string& name, Graphics& gfx, std::shared_ptr<ShaderManager> manager);
private:
	ModelBuilder builder;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

};

