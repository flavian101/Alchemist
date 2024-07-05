#pragma once
#include "models/Model.h"

class Grid: public Model
{
public:
	Grid(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> manager);
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

};

