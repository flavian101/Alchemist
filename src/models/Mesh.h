#pragma once
#include <memory>
#include <vector>
#include "Vertex.h"
#include "Graphics/Utilis.h"
#include "Transform.h"

class Graphics;
class Material;

class Mesh
{
public:

    Mesh(Graphics& gfx, const std::vector<unsigned short>& indices, const std::vector<Vertex>& vertices);
    ~Mesh();
    void Update(Graphics& gfx);
    void Bind(Graphics& gfx);
    void Render(Graphics& gfx,FXMMATRIX accumulatedTransform);
    void SetMaterial(std::shared_ptr<Material> material);
	std::shared_ptr<Material> GetMaterial() { return m_material; }
    void controlWindow();

    std::vector<Vertex> getVertices() { return m_vertices; }
    std::vector<unsigned short> getIndices() { return m_indices; }


    UINT getIndexCount() { return indexCount; };
    UINT getVertexCount() { return vertexCount; }

  

private:
    std::unique_ptr<Utils::VertexBuffer> vertexBuffer;
    std::unique_ptr<Utils::IndexBuffer> indexBuffer;
    std::shared_ptr<Material> m_material;
    mutable DirectX::XMFLOAT4X4 transform;
    std::shared_ptr<Transform> localTransform;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned short> m_indices;
    UINT indexCount;
    UINT vertexCount;

};
