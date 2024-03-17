#pragma once
#include "models\Model.h"

class Cube : public Model
{

public:
	Cube(Graphics& g, ShaderManager manager);

private:
	Graphics& m_graphic;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	
	
};

