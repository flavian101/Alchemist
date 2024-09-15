#pragma once
#include "models\ModelBuilder.h"
class ShaderManager;
class Graphics;
class Vertex;

class Plane 
{
public:
	Plane(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> maneger);

	void CreatePlane(float width, float depth,
		UINT m, UINT n);
	float calculateHeight(float x, float z);

	void Update(float deltaTime);
	void Render();
private:
	Graphics& m_graphic;
	ModelBuilder builder;
	

};

