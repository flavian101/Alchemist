#pragma once
#include <assimp/Exporter.hpp>
#include <memory>
#include "models/Model.h"
#include "models/Material.h"

class ModelExporter
{
public:
	ModelExporter(Model& model);
	~ModelExporter();
	void Export(const std::string& filepath);

private:
	Model& m_model;
};

