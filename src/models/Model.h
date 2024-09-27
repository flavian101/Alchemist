#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <stdafx.h>
#include <memory>

class ShaderManager;
class Graphics;
class Node;
class Mesh;
class ThirdPerson;
class Material;
class Vertex;

class Model 
{
public:
	Model(const std::string& name, std::shared_ptr<ShaderManager> shaderManager);
	~Model();

	

	void Update(Graphics& gfx,float deltaTime);
	void SetRootTransform(FXMMATRIX tf);
	void Render(Graphics& gfx) ;


	void controlWindow();

	
private:
	friend class SceneSerializer;

protected:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
};

