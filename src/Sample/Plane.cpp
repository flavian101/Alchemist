#include "Plane.h"

Plane::Plane(Graphics& g, ShaderManager maneger)
	:
	Model(g,maneger),
	m_graphic(g)
{
}

void Plane::CreatePlane()
{
	// Bottom Face
	vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f));
	vertices.push_back(Vertex( 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f));
	vertices.push_back(Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f));


	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	CreateMesh(indices, vertices);
}
