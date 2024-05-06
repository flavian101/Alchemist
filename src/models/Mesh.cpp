#include "Mesh.h"

Mesh::Mesh(Graphics& g)
	:
	m_graphics(g)
{
}

void Mesh::AddMeshPart(MeshParts parts)
{
	m_meshParts.emplace_back(m_graphics);
	m_meshParts.back().Initialize(parts.getIndices(), parts.getVertices());
}
void Mesh::Bind()
{
	for (auto& meshPart : m_meshParts)
	{
		meshPart.Bind();
	}
}

void Mesh::Render()
{
	for (auto& meshPart : m_meshParts)
	{
		m_graphics.Render(meshPart.IndexCount());
	}
}



