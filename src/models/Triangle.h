#pragma once
#include "Graphics/Mesh.h"
#include "Graphics/Vertex.h"

class Triangle
{
public:
	Triangle(Graphics& g);

	void Draw(Graphics& g, FXMVECTOR camPos, FXMVECTOR camTarget);
	Mesh getTriangle(Graphics& g);

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	DirectX::XMMATRIX modelTransform;
	DirectX::XMMATRIX Rotation;
	DirectX::XMMATRIX Scale;
	DirectX::XMMATRIX Translation;

};

