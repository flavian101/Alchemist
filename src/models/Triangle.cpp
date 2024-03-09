#include "Triangle.h"




Triangle::Triangle(Graphics& g)
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

	
}

void Triangle::Draw(Graphics& g, FXMVECTOR camPos, FXMVECTOR camTarget, float angle)
{
	modelTransform = XMMatrixIdentity();
	Scale = XMMatrixScaling(2.0f, 2.0f, 2.0f);
	Translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
    
	//modelTransform = XMMatrixMultiply(Scale,XMMatrixMultiply(Translation, Rotation));
    modelTransform = Scale * Translation;
	getTriangle(g).Draw(g,modelTransform ,camPos, camTarget);
}

Mesh Triangle::getTriangle(Graphics& g)
{
	return Mesh(g, indices, vertices,
		L"Assets/shader/VertexShader.cso",
		L"Assets/shader/PixelShader.cso"
		
	);
}
