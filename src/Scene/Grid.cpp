#include "Grid.h"
#include "models/Vertex.h"
#include "Graphics/Graphics.h"
#include "Scene/Shaders/ShaderManager.h"
#include "models/Mesh.h"

Grid::Grid(const std::string& name, Graphics& gfx)
	:GameObject(name),
	builder(name,XMMatrixIdentity())
{

    float gridSize = 1000.0f; // 100 units across (50 in each direction from center)
    int numLines = 215; // Odd number for center line
    float spacing = gridSize / (numLines - 1);

    for (int i = 0; i < numLines; ++i)
    {
        float position = -gridSize / 2.0f + i * spacing;
        float alpha = 1.0f - (std::abs(position) / (gridSize / 2.0f));

        // Vertical lines
        vertices.emplace_back(position, 0.0f, -gridSize / 2.0f, 1.0f,1.0f,1.0f,alpha);
        vertices.emplace_back(position, 0.0f, gridSize / 2.0f, 1.0f, 1.0f, 1.0f, alpha);

        // Horizontal lines
        vertices.emplace_back(-gridSize / 2.0f, 0.0f, position, 1.0f, 1.0f, 1.0f, alpha);
        vertices.emplace_back(gridSize / 2.0f, 0.0f, position, 1.0f, 1.0f, 1.0f, alpha);
    }

    for (unsigned short i = 0; i < vertices.size(); ++i)
    {
        indices.emplace_back(i);
    }
    auto mesh = new Mesh(gfx, indices, vertices);

    builder.CreateNode(gfx, 0, name, mesh, XMMatrixIdentity());
   
}

void Grid::Update(Graphics& gfx, float deltaTime)
{
    GameObject::Update(gfx, deltaTime);
    builder.Update(gfx, deltaTime);
}

void Grid::Render(Graphics& gfx)
{
    GameObject::Render(gfx);
    builder.Render(gfx);
}

void Grid::controlWindow(Graphics& gfx)
{
    builder.controlWindow(gfx);
}
