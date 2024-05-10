#include "Plane.h"

Plane::Plane(const std::string& name,Graphics& g, ShaderManager maneger)
	:
	Model(name,g,maneger),
	m_graphic(g)
{
	m_position = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	this->setTranslation(Math::XMVectorToFloat3(m_position));
	m_scale = XMVectorSet(20.0f, 1.0f, 20.0f, 0.0f);
	this->setScale(Math::XMVectorToFloat3(m_scale));
}

void Plane::CreatePlane(float width, float depth,
	UINT m, UINT n)
{
	UINT vertexCount = m * n;
	UINT faceCount = (m - 1) * (n - 1) * 2;

    float halfWidth = 0.5f * width;
    float halfDepth = 0.5f * depth;
    float dx = width / (n - 1);
    float dz = depth / (m - 1);
    for (UINT i = 0; i < m; ++i)
    {
        float z = halfDepth - i * dz;
        for (UINT j = 0; j < n; ++j)
        {
            float x = -halfWidth + j * dx;

            float r = (x + halfWidth) / width; 
            float g = (x + halfDepth) / width;
            float b = (z + halfDepth) / depth; 

            float height = calculateHeight(x, z);
            // Add vertex to the list
            vertices.push_back(Vertex(x, height, z, r, g, b));
        }
    }

    for (UINT i = 0; i < m - 1; ++i)
    {
        for (UINT j = 0; j < n - 1; ++j)
        {
            // Calculate indices for each quad
            UINT topLeft = i * n + j;
            UINT topRight = topLeft + 1;
            UINT bottomLeft = (i + 1) * n + j;
            UINT bottomRight = bottomLeft + 1;

            // Reverse the order of indices to create the quad in the opposite winding order
            indices.push_back(topLeft);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);

            indices.push_back(topRight);
            indices.push_back(bottomRight);
            indices.push_back(bottomLeft);
        }
    }

    // Now you have the vertices and indices with color values
    // You can use them to create the mesh
    CreateMesh(vertices, indices);
}

float Plane::calculateHeight(float x, float z)
{
    float frequency = 2.0f;
    float amplitude = 20.0f;
    return amplitude * sin(frequency * x) * cos(frequency * z);
}
