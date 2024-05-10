#include "Cube.h"




Cube::Cube(const std::string& name,Graphics& g, ShaderManager manager)
    :Model(name,g,manager),
    m_graphic(g)   
{
    m_position = XMVectorSet(0.0f,3.0f, 0.0f, 0.0f);
    this->setTranslation(Math::XMVectorToFloat3(m_position));
    m_orientation = XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f);
    this->setRotation(Math::XMVectorToFloat4(m_orientation));

}

void Cube::CreateCube()
{
    // Define vertices for the cube
    vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f)); // bottom-left-back
    vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f)); // top-left-back
    vertices.push_back(Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f)); // top-right-back
    vertices.push_back(Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f)); // bottom-right-back

    // Back face
    vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f)); // bottom-left-front
    vertices.push_back(Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f)); // bottom-right-front
    vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f)); // top-right-front
    vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f)); // top-left-front

    // Top Face
    vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f)); // top-left-back
    vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f)); // top-left-front
    vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f)); // top-right-front
    vertices.push_back(Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f)); // top-right-back

    // Bottom Face
    vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f)); // bottom-left-back
    vertices.push_back(Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f)); // bottom-right-back
    vertices.push_back(Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f)); // bottom-right-front
    vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f)); // bottom-left-front

    // Left Face
    vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f)); // bottom-left-front
    vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f)); // top-left-front
    vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f)); // top-left-back
    vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f)); // bottom-left-back

    // Right Face
    vertices.push_back(Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f)); // bottom-right-back
    vertices.push_back(Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f)); // top-right-back
    vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f)); // top-right-front
    vertices.push_back(Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f)); // bottom-right-front

    indices.reserve(36); // Reserve space for efficiency

    // Front Face
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    // Back Face
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(7);

    // Top Face
    indices.push_back(8);
    indices.push_back(9);
    indices.push_back(10);
    indices.push_back(8);
    indices.push_back(10);
    indices.push_back(11);

    // Bottom Face
    indices.push_back(12);
    indices.push_back(13);
    indices.push_back(14);
    indices.push_back(12);
    indices.push_back(14);
    indices.push_back(15);

    // Left Face
    indices.push_back(16);
    indices.push_back(17);
    indices.push_back(18);
    indices.push_back(16);
    indices.push_back(18);
    indices.push_back(19);

    // Right Face
    indices.push_back(20);
    indices.push_back(21);
    indices.push_back(22);
    indices.push_back(20);
    indices.push_back(22);
    indices.push_back(23);

    //CreateMesh(vertices, indices);
    TexturedMesh(vertices, indices, "Assets/textures/metalpanel.jpg", 0);
}

void Cube::Move(const DirectX::XMVECTOR& direction, float speed, float deltaTime)
{
    XMVECTOR normalizedDirection = XMVector3Normalize(direction);
    XMVECTOR movement = normalizedDirection * speed * deltaTime;

   
    m_position = XMVectorAdd(m_position, movement);
    this->setTranslation(Math::XMVectorToFloat3(m_position));
}

void Cube::Rotate(float angleInDegrees)
{

    float angleInRadians = XMConvertToRadians(angleInDegrees);
    DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    // Create a rotation quaternion
    DirectX::XMVECTOR rotationQuaternion = DirectX::XMQuaternionRotationAxis(rotationAxis, angleInRadians);

    // Apply the rotation to the character's orientation
    m_orientation = DirectX::XMQuaternionMultiply(m_orientation, rotationQuaternion);

    // Normalize the orientation quaternion
    m_orientation = DirectX::XMQuaternionNormalize(m_orientation);

    this->setRotation(Math::XMVectorToFloat4(m_orientation));
}






