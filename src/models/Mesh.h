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

    void Update(Graphics& gfx);
    void Bind(Graphics& gfx);
    void Render(Graphics& gfx,FXMMATRIX accumulatedTransform);
    void SetMaterial(std::shared_ptr<Material> material);
    void controlWindow();

    UINT getIndexCount() { return indexCount; };
    UINT getVertexCount() { return vertexCount; }

  

private:
    std::unique_ptr<Utils::VertexBuffer> vertexBuffer;
    std::unique_ptr<Utils::IndexBuffer> indexBuffer;
    std::shared_ptr<Material> m_material;
    mutable DirectX::XMFLOAT4X4 transform;
    std::shared_ptr<Transform> localTransform;
    UINT indexCount;
    UINT vertexCount;

};
