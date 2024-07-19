#pragma once
#include "models\Model.h"
class ShaderManager;
class Graphics;
class Vertex;

class Plane :public Model
{
public:
	Plane(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> maneger);

	void CreatePlane(float width, float depth,
		UINT m, UINT n);
	float calculateHeight(float x, float z);
private:
	Graphics& m_graphic;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

};

