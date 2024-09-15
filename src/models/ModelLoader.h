#pragma once
#include "Scene/RenderableObject.h"
#include <string>
#include <filesystem>
#include <vector>
#include <memory>
#include <models/Model.h>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "models/Material.h"

class Graphics;
class Vertex;
class Mesh;


class ModelLoader
{
public:
	ModelLoader(const std::string& filepath, Graphics& g, std::shared_ptr<ShaderManager> manager);
	void Update(float deltaTime);
	void Render();
	//std::string& GetPath() { return m_filepath; }
	//void setPath(std::string& path);

	void controlWindow();

private:
	void ProcessNode(aiNode* node, const aiScene* scene, const DirectX::FXMMATRIX& parentTransform);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, const DirectX::FXMMATRIX& nodeTransform);
	bool LoadMaterialTextures(aiMaterial* mat, aiTextureType type, Material::TextureType textureType, Material& material);
	
	
private:
	Graphics& m_graphics;
	std::string basePath;
	Model m_model;

};

