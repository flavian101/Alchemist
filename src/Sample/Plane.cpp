#include "Plane.h"

Plane::Plane(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> maneger)
	:
	Model(name,g,std::move(maneger)),
	m_graphic(g)
{
	m_position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	this->setTranslation(Math::XMVectorToFloat3(m_position));
	m_scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	this->setScale(Math::XMVectorToFloat3(m_scale));
}

void Plane::CreatePlane(float width, float depth, UINT m, UINT n)
{
   
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

            // Placeholder normal (will be computed later)
            vertices.push_back(Vertex(x, height, z, u, v, 0.0f, 0.0f, 0.0f));
        }
    }

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

    // Compute normals
    for (UINT i = 0; i < m - 1; ++i)
    {
        for (UINT j = 0; j < n - 1; ++j)
        {
            UINT topLeft = i * n + j;
            UINT topRight = topLeft + 1;
            UINT bottomLeft = (i + 1) * n + j;
            UINT bottomRight = bottomLeft + 1;

            XMFLOAT3 v0 = vertices[topLeft].pos3TexNorm.pos;
            XMFLOAT3 v1 = vertices[topRight].pos3TexNorm.pos;
            XMFLOAT3 v2 = vertices[bottomLeft].pos3TexNorm.pos;
            XMFLOAT3 v3 = vertices[bottomRight].pos3TexNorm.pos;

            XMVECTOR edge1 = XMVectorSubtract(XMLoadFloat3(&v1), XMLoadFloat3(&v0));
            XMVECTOR edge2 = XMVectorSubtract(XMLoadFloat3(&v2), XMLoadFloat3(&v0));
            XMVECTOR edge3 = XMVectorSubtract(XMLoadFloat3(&v3), XMLoadFloat3(&v1));

            XMVECTOR normal1 = XMVector3Cross(edge1, edge2);
            XMVECTOR normal2 = XMVector3Cross(edge3, edge2);

            normal1 = XMVector3Normalize(normal1);
            normal2 = XMVector3Normalize(normal2);

            // Accumulate normals for smooth shading
            XMFLOAT3 n1, n2;
            XMStoreFloat3(&n1, normal1);
            XMStoreFloat3(&n2, normal2);

            vertices[topLeft].pos3TexNorm.norm.x += n1.x;
            vertices[topLeft].pos3TexNorm.norm.y += n1.y;
            vertices[topLeft].pos3TexNorm.norm.z += n1.z;
            vertices[topRight].pos3TexNorm.norm.x += n1.x;
            vertices[topRight].pos3TexNorm.norm.y += n1.y;
            vertices[topRight].pos3TexNorm.norm.z += n1.z;
            vertices[bottomLeft].pos3TexNorm.norm.x += n1.x;
            vertices[bottomLeft].pos3TexNorm.norm.y += n1.y;
            vertices[bottomLeft].pos3TexNorm.norm.z += n1.z;

            vertices[topRight].pos3TexNorm.norm.x += n2.x;
            vertices[topRight].pos3TexNorm.norm.y += n2.y;
            vertices[topRight].pos3TexNorm.norm.z += n2.z;
            vertices[bottomLeft].pos3TexNorm.norm.x += n2.x;
            vertices[bottomLeft].pos3TexNorm.norm.y += n2.y;
            vertices[bottomLeft].pos3TexNorm.norm.z += n2.z;
            vertices[bottomRight].pos3TexNorm.norm.x += n2.x;
            vertices[bottomRight].pos3TexNorm.norm.y += n2.y;
            vertices[bottomRight].pos3TexNorm.norm.z += n2.z;
        }
    }

    // Normalize the accumulated normals
    for (UINT i = 0; i < vertices.size(); ++i)
    {
        XMVECTOR normal = XMLoadFloat3(&vertices[i].pos3TexNorm.norm);
        normal = XMVector3Normalize(normal);
        XMStoreFloat3(&vertices[i].pos3TexNorm.norm, normal);
    }

    // Create the mesh with the generated vertices and indices
    TexturedMesh(vertices, indices, "Assets/textures/hay.jpg", 0u);
}


float Plane::calculateHeight(float x, float z)
{
    float frequency =1.0f;
    float amplitude = 3.0f;
    return amplitude * sin(frequency * x) * cos(frequency * z);
}
