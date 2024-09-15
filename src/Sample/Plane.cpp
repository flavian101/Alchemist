#include "Plane.h"
#include "models/Vertex.h"
#include "Scene/Shaders/ShaderManager.h"

Plane::Plane(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> maneger)
	:
	m_graphic(g),
    builder(name, g, std::move(maneger))

{
   

	//m_position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//this->setTranslation(Math::XMVectorToFloat3(m_position));
	//m_scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	//this->setScale(Math::XMVectorToFloat3(m_scale));
}

void Plane::CreatePlane(float width, float depth, UINT m, UINT n)
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
            vertices.push_back(Vertex(x, height, z, u, v, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
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
            indices.push_back(topLeft);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);

            // Second triangle (reversed winding order)
            indices.push_back(topRight);
            indices.push_back(bottomRight);
            indices.push_back(bottomLeft);
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

            XMFLOAT3 v0 = vertices[topLeft].pos3TexNormTan.pos;
            XMFLOAT3 v1 = vertices[topRight].pos3TexNormTan.pos;
            XMFLOAT3 v2 = vertices[bottomLeft].pos3TexNormTan.pos;
            XMFLOAT3 v3 = vertices[bottomRight].pos3TexNormTan.pos;

            XMFLOAT2 uv0 = vertices[topLeft].pos3TexNormTan.tex;
            XMFLOAT2 uv1 = vertices[topRight].pos3TexNormTan.tex;
            XMFLOAT2 uv2 = vertices[bottomLeft].pos3TexNormTan.tex;

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
            vertices[topLeft].pos3TexNormTan.norm = Math::XMFloat3Add(vertices[topLeft].pos3TexNormTan.norm, n1);
            vertices[topRight].pos3TexNormTan.norm = Math::XMFloat3Add(vertices[topRight].pos3TexNormTan.norm, n1);
            vertices[bottomLeft].pos3TexNormTan.norm = Math::XMFloat3Add(vertices[bottomLeft].pos3TexNormTan.norm, n1);
            vertices[topRight].pos3TexNormTan.norm = Math::XMFloat3Add(vertices[topRight].pos3TexNormTan.norm, n2);
            vertices[bottomLeft].pos3TexNormTan.norm = Math::XMFloat3Add(vertices[bottomLeft].pos3TexNormTan.norm, n2);
            vertices[bottomRight].pos3TexNormTan.norm = Math::XMFloat3Add(vertices[bottomRight].pos3TexNormTan.norm, n2);

            // Accumulate tangents
            vertices[topLeft].pos3TexNormTan.tangent = Math::XMFloat3Add(vertices[topLeft].pos3TexNormTan.tangent, t);
            vertices[topRight].pos3TexNormTan.tangent = Math::XMFloat3Add(vertices[topRight].pos3TexNormTan.tangent, t);
            vertices[bottomLeft].pos3TexNormTan.tangent = Math::XMFloat3Add(vertices[bottomLeft].pos3TexNormTan.tangent, t);
            vertices[bottomRight].pos3TexNormTan.tangent = Math::XMFloat3Add(vertices[bottomRight].pos3TexNormTan.tangent, t);
        }
    }

    // Normalize the accumulated normals and tangents
    for (UINT i = 0; i < vertices.size(); ++i)
    {
        XMVECTOR normal = XMLoadFloat3(&vertices[i].pos3TexNormTan.norm);
        XMVECTOR tangent = XMLoadFloat3(&vertices[i].pos3TexNormTan.tangent);

        normal = XMVector3Normalize(normal);
        tangent = XMVector3Normalize(tangent);

        XMStoreFloat3(&vertices[i].pos3TexNormTan.norm, normal);
        XMStoreFloat3(&vertices[i].pos3TexNormTan.tangent, tangent);
    }

    // Create the mesh with the generated vertices and indices
    auto material = builder.CreateMaterial(
        "Assets/textures/hay.jpg",
        "Assets/textures/N_hay.png",
        "Assets/textures/M_hay.png",
        "Assets/textures/S_hay.png",
        "Assets/textures/AO_hay.png");

    builder.createMesh(indices, vertices, material);
   
}


float Plane::calculateHeight(float x, float z)
{
    float frequency =1.0f;
    float amplitude = 3.0f;
    return amplitude * sin(frequency * x) * cos(frequency * z);
}

void Plane::Update(float deltaTime)
{
    builder.getModel().Update(deltaTime);
}

void Plane::Render()
{
    builder.getModel().Render();
}
