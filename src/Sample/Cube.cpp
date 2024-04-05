#include "Cube.h"




Cube::Cube(Graphics& g, ShaderManager manager)
    :Model(g,manager),
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
	vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f)); // bottom-left-back
	vertices.push_back(Vertex(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f)); // top-left-back
	vertices.push_back(Vertex(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f)); // top-right-back
	vertices.push_back(Vertex(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f)); // bottom-right-back
	vertices.push_back(Vertex(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f)); // bottom-left-front
	vertices.push_back(Vertex(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f)); // top-left-front
	vertices.push_back(Vertex(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f)); // top-right-front
	vertices.push_back(Vertex(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f)); // bottom-right-front

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);
    // Front face
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(4);
    indices.push_back(7);
    indices.push_back(6);
    // Left face
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(0);
    // Right face
    indices.push_back(3);
    indices.push_back(2);
    indices.push_back(6);
    indices.push_back(3);
    indices.push_back(6);
    indices.push_back(7);
    // Top face
    indices.push_back(1);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(1);
    indices.push_back(6);
    indices.push_back(2);
    // Bottom face
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(3);
    indices.push_back(7);

    CreateMesh(indices, vertices);
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





