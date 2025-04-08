#pragma once
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <memory>
#include "models/Model.h"
#include "models/Material.h"

class ModelExporter
{
public:
	ModelExporter(Model& model);
	~ModelExporter();
	void Export(const std::string& filepath);
	void ExportNodeTransformations(aiNode* ai_node, const Node* node);


private:
	Model& m_model;
};

