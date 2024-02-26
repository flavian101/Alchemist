#pragma once
#include "DirectXMath.h"
#include "Utilis.h"
#include "Graphics.h"


class Mesh
{
public:
	Mesh(Graphics& g, std::vector<unsigned short>& indices, std::vector<Vertex>& v
	, LPCWSTR vertexShader, LPCWSTR pixelShader);

	void Draw(Graphics& g);

private:
	UINT indexCount;

};

