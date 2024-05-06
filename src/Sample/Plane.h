#pragma once
#include "models\Model.h"


class Plane :public Model
{
public:
	Plane(const std::string& name,Graphics& g, ShaderManager maneger);

	void CreatePlane(float width, float depth,
		UINT m, UINT n);
	float calculateHeight(float x, float z);
private:
	Graphics& m_graphic;
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

};

