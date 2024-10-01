#include "Player.h"
#include "Scene/Shaders/ShaderManager.h"
#include "Graphics/Graphics.h"
#include "models/Vertex.h"
#include "models/Mesh.h"

Player::Player(const std::string& name, Graphics& gfx, std::shared_ptr<ShaderManager> shaderManager)
    :GameObject(name),
    builder(name, shaderManager,XMMatrixIdentity())
{
    // m_position = XMVectorSet(1.0f, 2.0f, 0.0f, 0.0f);
    // this->setTranslation(Math::XMVectorToFloat3(m_position));
    // m_orientation = XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f);
    // this->setRotation(Math::XMVectorToFloat4(m_orientation));

    vertices.reserve(24);
      // Front face
    vertices.emplace_back(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f); // bottom-left-back
    vertices.emplace_back(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f); // top-left-back
    vertices.emplace_back( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f); // top-right-back
    vertices.emplace_back( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f); // bottom-right-back

    // Back face
    vertices.emplace_back(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f); // bottom-left-front
    vertices.emplace_back( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f); // bottom-right-front
    vertices.emplace_back( 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f); // top-right-front
    vertices.emplace_back(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f); // top-left-front

    // Top face
    vertices.emplace_back(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f); // top-left-back
    vertices.emplace_back(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f); // top-left-front
    vertices.emplace_back( 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f); // top-right-front
    vertices.emplace_back( 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f); // top-right-back

    // Bottom face
    vertices.emplace_back(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f); // bottom-left-back
    vertices.emplace_back( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f); // bottom-right-back
    vertices.emplace_back( 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f); // bottom-right-front
    vertices.emplace_back(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f); // bottom-left-front

    // Left face
    vertices.emplace_back(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f); // bottom-left-front
    vertices.emplace_back(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f); // top-left-front
    vertices.emplace_back(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f); // top-left-back
    vertices.emplace_back(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f); // bottom-left-back

    // Right face
    vertices.emplace_back(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); // bottom-right-back
    vertices.emplace_back(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); // top-right-back
    vertices.emplace_back(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); // top-right-front
    vertices.emplace_back(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); // bottom-right-front


    indices.reserve(36); // Reserve space for efficiency

    // Front face
    indices.emplace_back(0);
    indices.emplace_back(1);
    indices.emplace_back(2);
    indices.emplace_back(0);
    indices.emplace_back(2);
    indices.emplace_back(3);

    // Back face
    indices.emplace_back(4);
    indices.emplace_back(5);
    indices.emplace_back(6);
    indices.emplace_back(4);
    indices.emplace_back(6);
    indices.emplace_back(7);

    // Top face
    indices.emplace_back(8);
    indices.emplace_back(9);
    indices.emplace_back(10);
    indices.emplace_back(8);
    indices.emplace_back(10);
    indices.emplace_back(11);

    // Bottom face
    indices.emplace_back(12);
    indices.emplace_back(13);
    indices.emplace_back(14);
    indices.emplace_back(12);
    indices.emplace_back(14);
    indices.emplace_back(15);

    // Left face
    indices.emplace_back(16);
    indices.emplace_back(17);
    indices.emplace_back(18);
    indices.emplace_back(16);
    indices.emplace_back(18);
    indices.emplace_back(19);

    // Right face
    indices.emplace_back(20);
    indices.emplace_back(21);
    indices.emplace_back(22);
    indices.emplace_back(20);
    indices.emplace_back(22);
    indices.emplace_back(23);

    auto mesh = new Mesh(gfx, indices, vertices);

    auto material = builder.CreateMaterial(gfx,
        "Assets/textures/stone wall.jpg",
        "Assets/textures/N_stone wall.jpg",
        "Assets/textures/M_stone wall.jpg",
        "Assets/textures/S_stone wall.jpg",
        "Assets/textures/AO_stone wall.jpg");

    mesh->SetMaterial(material);
    builder.CreateNode(gfx, 1, name, mesh, XMMatrixIdentity());

}
void Player::Update(Graphics& gfx,float deltaTime)
{
    GameObject::Update(gfx, deltaTime);
    builder.Update(gfx,deltaTime);
}
void Player::Render(Graphics& gfx)
{
    GameObject::Render(gfx);
    builder.Render(gfx);
}

void Player::controlWindow(Graphics& gfx)
{
    builder.controlWindow(gfx);
}
