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

    Mesh(Graphics& g, const std::vector<unsigned short>& indices, const std::vector<Vertex>& vertices);

    void Update(float deltaTime);
    void Bind();
    void Render();
    void SetMaterial(std::shared_ptr<Material> material);
    void controlWindow();

    UINT getIndexCount() { return indexCount; };
    UINT getVertexCount() { return vertexCount; }

    void SetLocalTransform(const FXMMATRIX& transform);
    void setLocalTranslation(const XMFLOAT3& translation);
    void setLocalRotation(const XMFLOAT4& rotation);
    void setLocalScale(const XMFLOAT3& scale);


private:
    Graphics& m_graphics;
    std::unique_ptr<Utils::VertexBuffer> vertexBuffer;
    std::unique_ptr<Utils::IndexBuffer> indexBuffer;
    std::shared_ptr<Material> m_material;
    std::unique_ptr <Transform> localTransform;
    UINT indexCount;
    UINT vertexCount;

};
