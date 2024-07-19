#include "MeshParts.h"
#include "datapack/SceneSerializer.h"
#include "Graphics/Graphics.h"

MeshParts::MeshParts(Graphics& g)
	:
	m_graphics(g),
	vertexBuffer(std::make_unique<Utils::VertexBuffer>(g)),
	indexBuffer(std::make_unique < Utils::IndexBuffer>(g))
{}

void MeshParts::Initialize(const std::vector<unsigned short>& indices,const  std::vector<Vertex>& vertices)
{
	this->m_indices = indices;
	indexCount = static_cast<UINT>(m_indices.size());
	indexBuffer->InitializeIndexBuffer(m_indices);

	this->m_vertices = vertices;
	vertexBuffer->InitializeVertexBuffer(m_vertices);

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
