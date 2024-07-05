#include "Grid.h"

Grid::Grid(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> manager)
	:
	Model(name, g, manager)
{

    float gridSize = 1000.0f; // 100 units across (50 in each direction from center)
    int numLines = 215; // Odd number for center line
    float spacing = gridSize / (numLines - 1);

    for (int i = 0; i < numLines; ++i)
    {
        float position = -gridSize / 2.0f + i * spacing;
        float alpha = 1.0f - (std::abs(position) / (gridSize / 2.0f));

        // Vertical lines
        vertices.push_back(Vertex(position, 0.0f, -gridSize / 2.0f, 1.0f,1.0f,1.0f,alpha));
        vertices.push_back(Vertex(position, 0.0f, gridSize / 2.0f, 1.0f, 1.0f, 1.0f, alpha));

        // Horizontal lines
        vertices.push_back(Vertex(-gridSize / 2.0f, 0.0f, position, 1.0f, 1.0f, 1.0f, alpha));
        vertices.push_back(Vertex(gridSize / 2.0f, 0.0f, position, 1.0f, 1.0f, 1.0f, alpha));
    }

    for (unsigned short i = 0; i < vertices.size(); ++i)
    {
        indices.push_back(i);
    }
    CreateMesh(vertices, indices);
}
