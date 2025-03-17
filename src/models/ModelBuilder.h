#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Model.h"
class Graphics;
class ShaderManager;
class Vertex;

class ModelBuilder : public Model
{
public:
	ModelBuilder(const std::string& name,XMMATRIX transform_in);

	void CreateNode(Graphics& gfx, int nextID, const std::string& name, Mesh* meshPart, FXMMATRIX transform_in);
	std::shared_ptr<Material> CreateMaterialFromTexture(Graphics& gfx,const char* albedoPath = nullptr, const char* normalPath = nullptr, const char* metallicPath = nullptr,
		const char* roughnessPath = nullptr, const char* aoPath = nullptr);
	void Update(Graphics& gfx, float deltaTime) override;
	void Render(Graphics& gfx)override;
private:	


};

