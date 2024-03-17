#pragma once
#include "DirectXMath.h"
#include "Graphics\Utilis.h"
//#include "ConstantBuffer.h"


class Mesh
{
public:
	Mesh(Graphics& g);

	void setIndices(std::vector<unsigned short>& indices);
	void setVertices(std::vector<Vertex>& vertices);
	std::vector<unsigned short>& getIndices();
	std::vector<Vertex>& getVertices();
	void Bind();

private:
	Graphics& m_graphics;
	Utils::VertexBuffer vertexBuffer;
	Utils::IndexBuffer indexBuffer;
	Utils::Topology topology;
	std::vector<unsigned short> m_indices;
	std::vector<Vertex> m_vertices;
	UINT indexCount =0u;
	
};

