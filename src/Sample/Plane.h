#pragma once
#include "models\ModelBuilder.h"
#include "Scene/GameObject.h"

class ShaderManager;
class Graphics;
class Vertex;
class Mesh;

class Plane :public GameObject
{
public:
	Plane(const std::string& name);

	void CreatePlane(Graphics& gfx,float width, float depth,
		UINT m, UINT n);
	float calculateHeight(float x, float z);

	void Update(Graphics& gfx,float deltaTime);
	void Render(Graphics& gfx);
	void controlWindow(Graphics& gfx);
private:
	ModelBuilder builder;
	

};

