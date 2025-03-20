#include "Mesh.h"
#include "Material.h"
#include "Graphics/Graphics.h"
#include "imgui/imgui.h"

Mesh::Mesh(Graphics& gfx, const std::vector<unsigned short>& indices, const std::vector<Vertex>& vertices)
    :
    vertexBuffer(std::make_unique<Utils::VertexBuffer>(gfx, vertices)),
    indexBuffer(std::make_unique<Utils::IndexBuffer>(gfx, indices)),
    indexCount(static_cast<UINT>(indices.size())),
    vertexCount(static_cast<UINT>(vertices.size())),
    localTransform(std::make_unique<Transform>(gfx)),
    m_material(std::make_shared<Material>(gfx))
{
}

Mesh::~Mesh()
{
}

void Mesh::Update(Graphics& gfx)
{
    if (m_material)
    {
        m_material->Update(gfx);
    }
}

void Mesh::Bind(Graphics& gfx)
{
    vertexBuffer->Bind(gfx);
    indexBuffer->Bind(gfx);

}
void Mesh::Render(Graphics& gfx,FXMMATRIX accumulatedTransform)
{
    localTransform->UpdateFromTransform(gfx, accumulatedTransform);
    localTransform->BindConstantBuffer(gfx);
    if (m_material)
    {
        m_material->Bind(gfx); 
    }
    gfx.Render(indexCount);
    m_material->UnbindMaterials(gfx);
}

void Mesh::SetMaterial(std::shared_ptr<Material> material)
{
    this->m_material = material;
}

void Mesh::controlWindow()
{
    
    if (m_material)
    {
        m_material->controlWindow();
    }
    localTransform->controlWindow();
}
