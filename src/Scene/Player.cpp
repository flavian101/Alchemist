#include "Player.h"

Player::Player(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> shaderManager)
	:
	Model(name, g, shaderManager)
{
    m_position = XMVectorSet(1.0f, 2.0f, 0.0f, 0.0f);
    this->setTranslation(Math::XMVectorToFloat3(m_position));
    m_orientation = XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f);
    this->setRotation(Math::XMVectorToFloat4(m_orientation));

    // Front face
    vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f)); // bottom-left-back
    vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f)); // top-left-back
    vertices.push_back(Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f)); // top-right-back
    vertices.push_back(Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f)); // bottom-right-back

    // Back face
    vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f)); // bottom-left-front
    vertices.push_back(Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f)); // bottom-right-front
    vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)); // top-right-front
    vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f)); // top-left-front

    // Top face
    vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f)); // top-left-back
    vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f)); // top-left-front
    vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f)); // top-right-front
    vertices.push_back(Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f)); // top-right-back

    // Bottom face
    vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f)); // bottom-left-back
    vertices.push_back(Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f)); // bottom-right-back
    vertices.push_back(Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f)); // bottom-right-front
    vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f)); // bottom-left-front

    // Left face
    vertices.push_back(Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f)); // bottom-left-front
    vertices.push_back(Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f)); // top-left-front
    vertices.push_back(Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f)); // top-left-back
    vertices.push_back(Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f)); // bottom-left-back

    // Right face
    vertices.push_back(Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f)); // bottom-right-back
    vertices.push_back(Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f)); // top-right-back
    vertices.push_back(Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f)); // top-right-front
    vertices.push_back(Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f)); // bottom-right-front

    indices.reserve(36); // Reserve space for efficiency

    // Front face
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    // Back face
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(7);

    // Top face
    indices.push_back(8);
    indices.push_back(9);
    indices.push_back(10);
    indices.push_back(8);
    indices.push_back(10);
    indices.push_back(11);

    // Bottom face
    indices.push_back(12);
    indices.push_back(13);
    indices.push_back(14);
    indices.push_back(12);
    indices.push_back(14);
    indices.push_back(15);

    // Left face
    indices.push_back(16);
    indices.push_back(17);
    indices.push_back(18);
    indices.push_back(16);
    indices.push_back(18);
    indices.push_back(19);

    // Right face
    indices.push_back(20);
    indices.push_back(21);
    indices.push_back(22);
    indices.push_back(20);
    indices.push_back(22);
    indices.push_back(23);


    //CreateMesh(vertices, indices);
    TexturedMesh(vertices, indices, "Assets/textures/stone wall.jpg", 0u);
}
//
//void Player::Update(float deltaTime)
//{
//}
//
//void Player::Render()
//{
//}
//