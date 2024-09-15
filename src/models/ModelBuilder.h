#pragma once
#include <memory>
#include <vector>
#include <string>
#include "Model.h"
class Graphics;
class ShaderManager;
class Vertex;

class ModelBuilder
{
public:
	ModelBuilder(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> manager);

	void createMesh(const std::vector<unsigned short>& indices, const std::vector<Vertex>& vertices, std::shared_ptr<Material> material);
	std::shared_ptr<Material> CreateMaterial(const char* albedoPath = nullptr, const char* normalPath = nullptr, const char* metallicPath = nullptr,
		const char* roughnessPath = nullptr, const char* aoPath = nullptr);
	Model& getModel();
private:
	Graphics& m_graphics;
	
	std::string m_name;
	Model m_model;


};

