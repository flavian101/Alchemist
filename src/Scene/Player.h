#pragma once
#include "models/Model.h"


class Player : public Model
{
public:
	Player(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> shaderManager);
	//void Update(float deltaTime)override;
	//void Render()override;

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;


};

