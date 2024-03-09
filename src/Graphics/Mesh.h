#pragma once
#include "DirectXMath.h"
#include "Utilis.h"
#include "Graphics.h"
#include "ConstantBuffer.h"


class Mesh
{
public:
	Mesh(Graphics& g, std::vector<unsigned short>& indices, std::vector<Vertex>& v
	, LPCWSTR vertexShader, LPCWSTR pixelShader);

	void Draw(Graphics& g, XMMATRIX modelMatrix, XMVECTOR camPos, XMVECTOR camTarget);

private:
	UINT indexCount;
	ConstantBuffer<cb_vsConstantBuffer> vsBuffer;
	//matrices
	XMMATRIX WVP;
	XMMATRIX Model;
};

