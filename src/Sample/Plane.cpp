#include "Plane.h"
#include "models/Vertex.h"
#include "Scene/Shaders/ShaderManager.h"
#include <MathUtils/MathUtils.h>
#include "models/Mesh.h"

Plane::Plane(const std::string& name, std::shared_ptr<ShaderManager> maneger)
    :GameObject(name),
    builder(name, std::move(maneger), XMMatrixIdentity())

{
   

	//m_position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//this->setTranslation(Math::XMVectorToFloat3(m_position));
	//m_scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	//this->setScale(Math::XMVectorToFloat3(m_scale));
}

void Plane::CreatePlane(Graphics& gfx,float width, float depth, UINT m, UINT n)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;

    UINT vertexCount = m * n;
    UINT faceCount = (m - 1) * (n - 1) * 2;

    float halfWidth = 0.5f * width;
    float halfDepth = 0.5f * depth;
    float dx = width / (n - 1);
    float dz = depth / (m - 1);
    float du = 1.0f / (n - 1);
    float dv = 1.0f / (m - 1);

    // Generate vertices
    for (UINT i = 0; i < m; ++i)
    {
        float z = halfDepth - i * dz;
        for (UINT j = 0; j < n; ++j)
        {
            float x = -halfWidth + j * dx;
            float u = j * du;
            float v = i * dv;
            float height = calculateHeight(x, z);

            // Placeholder normal and tangent (will be computed later)
            vertices.emplace_back(x, height, z, u, v, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

    // Generate indices
    for (UINT i = 0; i < m - 1; ++i)
    {
        for (UINT j = 0; j < n - 1; ++j)
        {
            UINT topLeft = i * n + j;
            UINT topRight = topLeft + 1;
            UINT bottomLeft = (i + 1) * n + j;
            UINT bottomRight = bottomLeft + 1;

            // First triangle (reversed winding order)
            indices.emplace_back(topLeft);
            indices.emplace_back(topRight);
            indices.emplace_back(bottomLeft);

            // Second triangle (reversed winding order)
            indices.emplace_back(topRight);
            indices.emplace_back(bottomRight);
            indices.emplace_back(bottomLeft);
        }
    }

    // Compute normals and tangents
    for (UINT i = 0; i < m - 1; ++i)
    {
        for (UINT j = 0; j < n - 1; ++j)
        {
            UINT topLeft = i * n + j;
            UINT topRight = topLeft + 1;
            UINT bottomLeft = (i + 1) * n + j;
            UINT bottomRight = bottomLeft + 1;

            XMFLOAT3 v0 = vertices[topLeft].posTexNormTan.pos;
            XMFLOAT3 v1 = vertices[topRight].posTexNormTan.pos;
            XMFLOAT3 v2 = vertices[bottomLeft].posTexNormTan.pos;
            XMFLOAT3 v3 = vertices[bottomRight].posTexNormTan.pos;

            XMFLOAT2 uv0 = vertices[topLeft].posTexNormTan.tex;
            XMFLOAT2 uv1 = vertices[topRight].posTexNormTan.tex;
            XMFLOAT2 uv2 = vertices[bottomLeft].posTexNormTan.tex;

            XMVECTOR edge1 = XMVectorSubtract(XMLoadFloat3(&v1), XMLoadFloat3(&v0));
            XMVECTOR edge2 = XMVectorSubtract(XMLoadFloat3(&v2), XMLoadFloat3(&v0));
            XMVECTOR edge3 = XMVectorSubtract(XMLoadFloat3(&v3), XMLoadFloat3(&v1));

            XMVECTOR normal1 = XMVector3Cross(edge1, edge2);
            XMVECTOR normal2 = XMVector3Cross(edge3, edge2);

            normal1 = XMVector3Normalize(normal1);
            normal2 = XMVector3Normalize(normal2);

            // Compute tangent
            XMVECTOR dUV1 = XMVectorSet(uv1.x - uv0.x, uv1.y - uv0.y, 0.0f, 0.0f);
            XMVECTOR dUV2 = XMVectorSet(uv2.x - uv0.x, uv2.y - uv0.y, 0.0f, 0.0f);

            float r = 1.0f / (XMVectorGetX(dUV1) * XMVectorGetY(dUV2) - XMVectorGetY(dUV1) * XMVectorGetX(dUV2));

            XMVECTOR tangent = XMVectorScale(
                XMVectorSubtract(
                    XMVectorScale(edge1, XMVectorGetY(dUV2)),
                    XMVectorScale(edge2, XMVectorGetY(dUV1))
                ),
                r
            );

            tangent = XMVector3Normalize(tangent);

            // Accumulate normals and tangents for smooth shading
            XMFLOAT3 n1, n2, t;
            XMStoreFloat3(&n1, normal1);
            XMStoreFloat3(&n2, normal2);
            XMStoreFloat3(&t, tangent);

            // Accumulate normals
            vertices[topLeft].posTexNormTan.norm = Math::XMFloat3Add(vertices[topLeft].posTexNormTan.norm, n1);
            vertices[topRight].posTexNormTan.norm = Math::XMFloat3Add(vertices[topRight].posTexNormTan.norm, n1);
            vertices[bottomLeft].posTexNormTan.norm = Math::XMFloat3Add(vertices[bottomLeft].posTexNormTan.norm, n1);
            vertices[topRight].posTexNormTan.norm = Math::XMFloat3Add(vertices[topRight].posTexNormTan.norm, n2);
            vertices[bottomLeft].posTexNormTan.norm = Math::XMFloat3Add(vertices[bottomLeft].posTexNormTan.norm, n2);
            vertices[bottomRight].posTexNormTan.norm = Math::XMFloat3Add(vertices[bottomRight].posTexNormTan.norm, n2);

            // Accumulate tangents
            vertices[topLeft].posTexNormTan.tangent = Math::XMFloat3Add(vertices[topLeft].posTexNormTan.tangent, t);
            vertices[topRight].posTexNormTan.tangent = Math::XMFloat3Add(vertices[topRight].posTexNormTan.tangent, t);
            vertices[bottomLeft].posTexNormTan.tangent = Math::XMFloat3Add(vertices[bottomLeft].posTexNormTan.tangent, t);
            vertices[bottomRight].posTexNormTan.tangent = Math::XMFloat3Add(vertices[bottomRight].posTexNormTan.tangent, t);
        }
    }

    // Normalize the accumulated normals and tangents
    for (UINT i = 0; i < vertices.size(); ++i)
    {
        XMVECTOR normal = XMLoadFloat3(&vertices[i].posTexNormTan.norm);
        XMVECTOR tangent = XMLoadFloat3(&vertices[i].posTexNormTan.tangent);

        normal = XMVector3Normalize(normal);
        tangent = XMVector3Normalize(tangent);

        XMStoreFloat3(&vertices[i].posTexNormTan.norm, normal);
        XMStoreFloat3(&vertices[i].posTexNormTan.tangent, tangent);
    }

    auto meshPart = new  Mesh(gfx, indices, vertices);
    // Create the mesh with the generated vertices and indices
    auto material = builder.CreateMaterial(gfx,
        "Assets/textures/hay.jpg",
        "Assets/textures/N_hay.png",
        "Assets/textures/M_hay.png",
        "Assets/textures/S_hay.png",
        "Assets/textures/AO_hay.png");

    meshPart->SetMaterial(material);

    builder.CreateNode(gfx, 1, "ground,", std::move(meshPart), XMMatrixIdentity());
}


float Plane::calculateHeight(float x, float z)
{
    float frequency =1.0f;
    float amplitude = 3.0f;
    return amplitude * sin(frequency * x) * cos(frequency * z);
}

void Plane::Update(Graphics& gfx,float deltaTime)
{
    GameObject::Update(gfx, deltaTime);
    builder.Update(gfx,deltaTime);
}

void Plane::Render(Graphics& gfx)
{
    GameObject::Render(gfx);
    builder.Render(gfx);
}

void Plane::controlWindow(Graphics& gfx)
{
    builder.controlWindow(gfx);
}
