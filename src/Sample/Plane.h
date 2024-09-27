#pragma once
#include "models\ModelBuilder.h"
class ShaderManager;
class Graphics;
class Vertex;

class Plane 
{
public:
	Plane(const std::string& name,std::shared_ptr<ShaderManager> maneger);

	void CreatePlane(Graphics& gfx,float width, float depth,
		UINT m, UINT n);
	float calculateHeight(float x, float z);

	void Update(Graphics& gfx,float deltaTime);
	void Render(Graphics& gfx);
private:
	ModelBuilder builder;
	

};

