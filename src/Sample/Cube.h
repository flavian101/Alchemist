#pragma once
#include "models\Model.h"

class Cube : public Model
{

public:
	Cube(Graphics& g, ShaderManager manager);
	void CreateCube();

	void Move(const DirectX::XMVECTOR& direction, float speed, float deltaTime);
	void Rotate(float angle, const DirectX::XMVECTOR& axis);


private:
	Graphics& m_graphic;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_orientation;
	
	
};

