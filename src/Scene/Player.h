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
	Player(const std::string& name, Graphics& gfx, std::shared_ptr<ShaderManager> shaderManager);
	void Update(Graphics& gfx,float deltaTime);
	void Render(Graphics& gfx);

private:
	ModelBuilder builder;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;


};

