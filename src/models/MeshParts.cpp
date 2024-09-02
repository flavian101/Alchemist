#include "MeshParts.h"
#include "datapack/SceneSerializer.h"
#include "Graphics/Graphics.h"

MeshParts::MeshParts(Graphics& g, const std::vector<unsigned short>& indices, const  std::vector<Vertex>& vertices)
	:
	m_graphics(g),
	vertexBuffer(std::make_unique<Utils::VertexBuffer>(g, vertices)),
	indexBuffer(std::make_unique<Utils::IndexBuffer>(g, indices))
{
	this->m_indices = indices;
	indexCount = static_cast<UINT>(m_indices.size());
	this->m_vertices = vertices;
}

MeshParts::~MeshParts()
{
	m_indices.clear();
	m_vertices.clear();
	
}
std::vector<unsigned short>& MeshParts::getIndices()
{
	return m_indices;
}

std::vector<Vertex>& MeshParts::getVertices() 
{
	return m_vertices;
}

UINT MeshParts::IndexCount()
{
	return indexCount;
}

void MeshParts::Bind()
{
	vertexBuffer->Bind();
	indexBuffer->Bind();
}
