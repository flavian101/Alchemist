#pragma once
#include "Graphics/Mesh.h"
#include "Graphics/Vertex.h"

class Triangle
{
public:
	Triangle(Graphics& g);

	void Draw(Graphics& g);
	Mesh getTriangle(Graphics& g);

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};

