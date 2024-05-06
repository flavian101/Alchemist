#include "Model.h"

Model::Model(const std::string& name,Graphics& g, ShaderManager shaderManager)
	:
	RenderableObject(name,g,shaderManager),
	m_graphics(g),
	m_mesh(g)
{
	
}

void Model::CreateMesh( const std::vector<Vertex>& vertices,const std::vector<unsigned short>& indices )
{
	MeshParts* part = new MeshParts(m_graphics);
	part->Initialize(indices, vertices);
	m_mesh.AddMeshPart(*part);
}

void Model::Render()
{
	m_transform.BindConstantBuffer();
	m_mesh.Bind();
	m_mesh.Render();
}

void Model::controlWindow()
{
	RenderableObject::controlWindow();
	ImGui::Separator();

	ImGui::Text("Information");
	ImGui::Text("name %s",m_name.c_str());
	//ImGui::Text("index count %d", m_mesh.getIndices().size());
	//ImGui::Text("Vertex count %d", m_mesh.getVertices().size());




}





