#pragma once
#include "Model.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

class ModelLoader  : public RenderableObject
{
public:
	ModelLoader( Graphics& g, std::shared_ptr<ShaderManager> manager);
	bool LoadModel(const std::string& filepath);
private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void Update(float deltaTime)override;
	void Render()override;
private:
	Graphics& m_graphics;
	std::vector<Mesh> m_meshes;
	MeshParts* part;

};

