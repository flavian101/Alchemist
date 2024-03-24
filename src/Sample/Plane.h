#pragma once
#include "models\Model.h"


class Plane :public Model
{
public:
	Plane(Graphics& g, ShaderManager maneger);

	void CreatePlane();
private:
	Graphics& m_graphic;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

};

