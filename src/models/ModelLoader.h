#pragma once
#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include <filesystem>


class ModelLoader  : public RenderableObject
{
public:
	ModelLoader( Graphics& g, std::shared_ptr<ShaderManager> manager);
	bool LoadModel(const std::string& filepath);
	void Update(float deltaTime)override;
	void Render()override;
private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh& ProcessMesh(aiMesh* mesh, const aiScene* scene);
	bool LoadMaterialTextures(aiMaterial* mat, aiTextureType type, Material::TextureType textureType, Material& material);

	
private:
	Graphics& m_graphics;
	std::string basePath;
	std::vector<Mesh*> m_meshes;
	MeshParts* part;

};

