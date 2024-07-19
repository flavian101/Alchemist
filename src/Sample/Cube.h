#pragma once
#include "models\Model.h"
class Graphics;
class ShaderManager;

class Cube : public Model
{

public:
	Cube(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> manager);
	void CreateCube();
	


private:
	Graphics& m_graphic;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	std::vector<Vertex> v;
	std::vector<unsigned short> i;

	
	
};

