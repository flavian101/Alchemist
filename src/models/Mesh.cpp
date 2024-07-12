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
	if (m_material)
	{
		m_material->Update();
		m_material->Bind(); // Bind the material
	}
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

void Mesh::SetMaterial(std::unique_ptr<Material> material)
{
	m_material = std::move(material);
}

void Mesh::controlWindow()
{
	ImGui::Text("Mesh");
	m_material->controlWindow();

}



