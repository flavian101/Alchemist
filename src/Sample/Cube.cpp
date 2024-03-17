#include "Cube.h"




Cube::Cube(Graphics& g, ShaderManager manager)
    :Model(g,manager),
    m_graphic(g)
   
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





