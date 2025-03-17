#pragma once
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
class Node;


class ModelLoader : public Model
{
public:
	ModelLoader(const std::string& filepath, Graphics& gfx);
	void Update(Graphics& gfx,float deltaTime);
	void Render(Graphics& gfx);
	//std::string& GetPath() { return m_filepath; }
	//void setPath(std::string& path);

private:
	std::unique_ptr<Mesh> parseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
	std::unique_ptr<Node> parseNode(int& nextId, const aiNode& node);
	bool LoadMaterialTextures(Graphics& gfx,const aiMaterial& mat, aiTextureType type, Material::TextureType textureType, Material& material);
	
	
private:
	std::string basePath;

};

