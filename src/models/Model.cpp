#include "Model.h"

Model::Model(Graphics& g, ShaderManager shaderManager)
	:
	RenderableObject(g,shaderManager),
	m_graphics(g),
	m_mesh(g)
{
	
}

void Model::CreateMesh(std::vector<unsigned short>& indices, std::vector<Vertex> vertices)
{
	m_mesh.setIndices(indices);
	m_mesh.setVertices(vertices);

}

void Model::Render()
{
	m_transform.BindConstantBuffer();
	m_mesh.Bind();
	m_graphics.Render(this->m_mesh.getIndices().size());
}





