#pragma once
#include "stdafx.h"
#include <vector>
#include <memory>
#include "Graphics/Utilis.h"
#include "Vertex.h"

class Graphics;
class SceneSerializer;

class MeshParts
{
public:
	MeshParts(Graphics& g, const std::vector<unsigned short>& indices, const  std::vector<Vertex>& vertices);
	~MeshParts();
	// Provide a copy constructor if needed
	MeshParts(const MeshParts& other) = default;

	// Provide a move constructor if needed
	MeshParts(MeshParts&& other) noexcept = default;

	// Provide a copy assignment operator if needed
	MeshParts& operator=(const MeshParts& other) = default;

	// Provide a move assignment operator if needed
	MeshParts& operator=(MeshParts&& other) noexcept = default;


	std::vector<unsigned short>& getIndices();
	std::vector<Vertex>& getVertices();

	UINT IndexCount();
	void Bind();

private:
	Graphics& m_graphics;
	std::unique_ptr<Utils::VertexBuffer> vertexBuffer;
	std::unique_ptr < Utils::IndexBuffer> indexBuffer;
	std::vector<unsigned short> m_indices;
	std::vector<Vertex> m_vertices;
	UINT indexCount = 0u;
	friend class SceneSerializer;

};

