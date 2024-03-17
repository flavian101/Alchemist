#include "Mesh.h"

Mesh::Mesh(Graphics& g)
	:
	m_graphics(g),
	vertexBuffer(g),
	indexBuffer(g),
	topology(m_graphics, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

void Mesh::setIndices(std::vector<unsigned short>& indices)
{
	this->m_indices = indices;
	indexCount = m_indices.size();
	indexBuffer.InitializeIndexBuffer(m_indices);
	
}

void Mesh::setVertices(std::vector<Vertex>& vertices)
{
	this->m_vertices = vertices;
	vertexBuffer.InitializeVertexBuffer(m_vertices);

}

std::vector<unsigned short>& Mesh::getIndices()
{
	return m_indices;
}

std::vector<Vertex>& Mesh::getVertices()
{
	return m_vertices;
}

void Mesh::Bind()
{
	topology.Bind();
	vertexBuffer.Bind();
	indexBuffer.Bind();


}


