#include "Plane.h"

Plane::Plane(const std::string& name,Graphics& g, ShaderManager maneger)
	:
	Model(name,g,maneger),
	m_graphic(g)
{
	m_position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	this->setTranslation(Math::XMVectorToFloat3(m_position));
	m_scale = XMVectorSet(20.0f, 0.0f, 20.0f, 0.0f);
	this->setScale(Math::XMVectorToFloat3(m_scale));
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
