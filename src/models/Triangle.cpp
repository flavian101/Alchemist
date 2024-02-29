#include "Triangle.h"




Triangle::Triangle(Graphics& g)
{
	vertices.push_back(Vertex(0.0f,   0.5f, 0.5f, 0.0f, 1.0f, 0.0f));
	vertices.push_back(Vertex(0.5f,  -0.5f, 0.5f, 0.0f, 0.0f, 1.0f));
	vertices.push_back(Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

}

void Triangle::Draw(Graphics& g)
{
	getTriangle(g).Draw(g);
}

Mesh Triangle::getTriangle(Graphics& g)
{
	return Mesh(g, indices, vertices,
		L"Assets/shader/VertexShader.cso",
		L"Assets/shader/PixelShader.cso"
		
	);
}
