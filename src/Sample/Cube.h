#pragma once
#include "models\Model.h"

class Cube : public Model
{

public:
	Cube(const std::string& name,Graphics& g, ShaderManager manager);
	void CreateCube();

	void Move(const DirectX::XMVECTOR& direction, float speed, float deltaTime);
	void Rotate(float angleInDegress);


private:
	Graphics& m_graphic;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	
	
};

