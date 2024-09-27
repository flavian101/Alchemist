#include "ModelBuilder.h"
#include "Mesh.h"
#include "Material.h"

ModelBuilder::ModelBuilder(const std::string& name, std::shared_ptr<ShaderManager> manager)
    :
    m_name(name),
    m_model(m_name, manager)
{
}

void ModelBuilder::createMesh(Graphics& gfx,const std::vector<unsigned short>& indices, const std::vector<Vertex>& vertices, std::shared_ptr<Material> material)
{
    auto mesh = std::make_unique<Mesh>(gfx, indices, vertices);
	mesh->SetMaterial(material);
}

std::shared_ptr<Material> ModelBuilder::CreateMaterial(Graphics& gfx,const char* albedoPath, const char* normalPath, const char* metallicPath, const char* roughnessPath, const char* aoPath)
{
	auto material = std::make_shared<Material>(gfx);
	if (albedoPath)
	{
		material->LoadTexture(gfx,Material::TextureType::Albedo, albedoPath);
		material->HasAlbedo(true);
	}
	if (normalPath)
	{
		material->LoadTexture(gfx,Material::TextureType::Normal, normalPath);
		material->HasNormal(true);;
	}
	if (metallicPath)
	{
		material->LoadTexture(gfx,Material::TextureType::Metallic, metallicPath);
		material->HasMetallic(true);
	}
	if (roughnessPath)
	{
		material->LoadTexture(gfx,Material::TextureType::Roughness, roughnessPath);
		material->HasRoughness(true); 
	}
	if (aoPath)
	{
		material->LoadTexture(gfx,Material::TextureType::AmbientOcclusion, aoPath);
		material->HasAO(true);
	}
	return material;
}

Model& ModelBuilder::getModel()
{
	return m_model;
}
