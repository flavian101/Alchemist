#pragma once
#include <string>
#include <memory>
#include <vector>
#include "models/Model.h"
#include "models/ModelBuilder.h"

class ShaderManager;
class Vertex;
class Graphics;


class Player
{
public:
	Player(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> shaderManager);
	void Update(float deltaTime);
	void Render();

private:
	ModelBuilder builder;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;


};

