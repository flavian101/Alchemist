#include "Cube.h"




Cube::Cube(Graphics& g, ShaderManager manager)
    :Model(g,manager),
    m_graphic(g)
   
{
    m_position = XMVectorSet(0.0f,3.0f, 0.0f, 0.0f);
    this->setTranslation(Math::XMVectorToFloat3(m_position));
    m_orientation = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    this->setRotation(Math::XMVectorToFloat3(m_orientation));

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
    
    //Calculate the displacement vector based on the direction , speed and delta time
    XMVECTOR displacement = direction * (speed * deltaTime);

    m_position += displacement;
    this->setTranslation(Math::XMVectorToFloat3(m_position));
}

void Cube::Rotate(float angle, const DirectX::XMVECTOR& axis)
{
    // Create a rotation quaternion based on the angle and axis
    DirectX::XMVECTOR rotation = DirectX::XMQuaternionRotationAxis(axis, angle);

    // Apply the rotation quaternion to the cube's orientation
    m_orientation = DirectX::XMQuaternionMultiply(rotation, m_orientation);

    // Normalize the orientation quaternion
    m_orientation = DirectX::XMQuaternionNormalize(m_orientation);

    this->setRotation(Math::XMVectorToFloat3(m_orientation));
}





