#pragma once
#include "Scene/RenderableObject.h"
#include <string>
#include <filesystem>
#include <vector>
#include <memory>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "models/Material.h"

class Graphics;
class Vertex;
class MeshParts;
class Mesh;


class ModelLoader  : public RenderableObject
{
public:
	ModelLoader( Graphics& g, std::shared_ptr<ShaderManager> manager);
	bool LoadModel(const std::string& filepath);
	void Update(float deltaTime)override;
	void Render()override;
	std::string& GetPath() { return m_filepath; }
	void setPath(std::string& path);

private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh& ProcessMesh(aiMesh* mesh, const aiScene* scene);
	bool LoadMaterialTextures(aiMaterial* mat, aiTextureType type, Material::TextureType textureType, Material& material);
	
	
private:
	Graphics& m_graphics;
	std::string m_filepath;
	std::string basePath;
	std::vector<Mesh*> m_meshes;
	MeshParts* part;

};

