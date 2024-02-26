#include "Mesh.h"

Mesh::Mesh(Graphics& g, std::vector<unsigned short>& indices, 
	std::vector<Vertex>& v, 
	LPCWSTR vertexShader, LPCWSTR pixelShader)
{

	indexCount = indices.size();
	Utils::IndexBuffer id(g, indices);
	id.Bind(g);

	Utils::VertexBuffer vb(g, v);
	vb.Bind(g);
	Utils::VertexShader vs(g, vertexShader);
	auto bytecode = vs.GetByteCode();
	vs.Bind(g);

	Utils::PixelShader ps(g, pixelShader);
	ps.Bind(g);

	Utils::InputLayout layout(g, bytecode);
	layout.Bind(g);

	Utils::Topology tp(g, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	tp.Bind(g);

}

void Mesh::Draw(Graphics& g)
{
	g.Render(indexCount);
}
