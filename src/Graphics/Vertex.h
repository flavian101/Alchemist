#pragma once
#include <DirectXMath.h>
#include <vector>

class Vertex
{
public:
	Vertex();
	//Vertex(float x, float y, float z,
	//	float u, float v,
	//	float nx, float ny, float nz)
	//	:
	//	pos3(x, y, z),
	//	tex(u, v),
	//	normals(nx, ny, nz),
	//	pos2(0.0f,0.0f),
	//	col(0.0f,0.0f,0.0f,0.0f)
	//{
	//
	//}
	//Vertex(float x, float y,
	//	float u, float v)
	//	:
	//	pos2(x, y),
	//	tex(u, v),
	//	pos3(0,0,0),
	//	normals(0.0f,0.0f,0.0f),
	//	col(0.0f, 0.0f, 0.0f, 0.0f)
	//{
	//
	//}
	Vertex(float x, float y,float z, float r, float g, float b)
		:
		pos3(x, y,z),
		col(r, g, b, 1.f)
		
	{

	}

private:
	DirectX::XMFLOAT3 pos3;
	//DirectX::XMFLOAT2 pos2;
	//DirectX::XMFLOAT2 tex;
	//DirectX::XMFLOAT3 normals;
	DirectX::XMFLOAT4 col;

};