#include "ModelBuilder.h"
#include "Mesh.h"
#include "Material.h"

ModelBuilder::ModelBuilder(const std::string& name,Graphics& g, std::shared_ptr<ShaderManager> manager)
    :
    m_graphics(g),
    m_name(name),
    m_model(m_name,m_graphics, manager)
{
}

void ModelBuilder::createMesh(const std::vector<unsigned short>& indices, const std::vector<Vertex>& vertices, std::shared_ptr<Material> material)
{
    auto mesh = std::make_unique<Mesh>(m_graphics, indices, vertices);
	mesh->SetMaterial(material);
    m_model.AddMesh(mesh.get());
}

std::shared_ptr<Material> ModelBuilder::CreateMaterial(const char* albedoPath, const char* normalPath, const char* metallicPath, const char* roughnessPath, const char* aoPath)
{
	auto material = std::make_shared<Material>(m_graphics);
	if (albedoPath)
	{
		material->LoadTexture(Material::TextureType::Albedo, albedoPath);
		material->HasAlbedo(true);
	}
	if (normalPath)
	{
		material->LoadTexture(Material::TextureType::Normal, normalPath);
		material->HasNormal(true);;
	}
	if (metallicPath)
	{
		material->LoadTexture(Material::TextureType::Metallic, metallicPath);
		material->HasMetallic(true);
	}
	if (roughnessPath)
	{
		material->LoadTexture(Material::TextureType::Roughness, roughnessPath);
		material->HasRoughness(true); 
	}
	if (aoPath)
	{
		material->LoadTexture(Material::TextureType::AmbientOcclusion, aoPath);
		material->HasAO(true);
	}
	return material;
}

Model& ModelBuilder::getModel()
{
	return m_model;
}
