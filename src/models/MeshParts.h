#pragma once
#include "DirectXMath.h"
#include "Graphics/Utilis.h"

class MeshParts
{
public:
	MeshParts(Graphics& g);

	void Initialize(const std::vector<unsigned short>& indices, const std::vector<Vertex>& vertices);
	std::vector<unsigned short>& getIndices();
	std::vector<Vertex>& getVertices();

	UINT IndexCount();
	void Bind();

private:
	Graphics& m_graphics;
	Utils::VertexBuffer vertexBuffer;
	Utils::IndexBuffer indexBuffer;
	Utils::Topology topology;
	std::vector<unsigned short> m_indices;
	std::vector<Vertex> m_vertices;
	UINT indexCount = 0u;
	friend class SceneSerializer;

};

