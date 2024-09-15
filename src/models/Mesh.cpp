#include "Mesh.h"
#include "Material.h"
#include "Graphics/Graphics.h"
#include "imgui/imgui.h"

Mesh::Mesh(Graphics& g, const std::vector<unsigned short>& indices, const std::vector<Vertex>& vertices)
    :
    m_graphics(g),
    vertexBuffer(std::make_unique<Utils::VertexBuffer>(g, vertices)),
    indexBuffer(std::make_unique<Utils::IndexBuffer>(g, indices)),
    indexCount(static_cast<UINT>(indices.size())),
    vertexCount(static_cast<UINT>(vertices.size())),
    localTransform(std::make_unique <Transform>(g))
{
}

void Mesh::Update(float deltaTime)
{
    localTransform->Update(deltaTime);
    //localTransform->PrintMatrix("mesh Transform", localTransform->GetTransform());
    if (m_material)
    {
        m_material->Update();
    }
}

void Mesh::Bind()
{
    vertexBuffer->Bind();
    indexBuffer->Bind();
    localTransform->BindConstantBuffer();

}
void Mesh::Render()
{
    if (m_material)
    {
        m_material->Bind(); 
    }
    m_graphics.Render(indexCount);
}

void Mesh::SetMaterial(std::shared_ptr<Material> material)
{
    m_material = material;
}

void Mesh::controlWindow()
{
    if (m_material)
    {
        m_material->controlWindow();
    }
    localTransform->controlWindow();
}

void Mesh::SetLocalTransform(const FXMMATRIX& transform)
{
    localTransform->setTransform(transform);
}

void Mesh::setLocalTranslation(const XMFLOAT3& translation)
{
    localTransform->SetPosition(translation);
}

void Mesh::setLocalRotation(const XMFLOAT4& rotation)
{
    localTransform->SetRotation(rotation);
}

void Mesh::setLocalScale(const XMFLOAT3& scale)
{
    localTransform->SetScale(scale);
}
