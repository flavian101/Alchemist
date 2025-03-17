#include "ModelBuilder.h"
#include "Mesh.h"
#include "Material.h"

ModelBuilder::ModelBuilder(const std::string& name,XMMATRIX transform_in)
    :
	Model(m_name)
{
	int nextID = 0;
	pRoot = std::make_unique<Node>(nextID, name, transform_in);
}

void ModelBuilder::CreateNode(Graphics& gfx,int nextID, const std::string& name, Mesh* meshPart, FXMMATRIX transform_in)
{
	auto childNode= std::make_unique<Node>(nextID, name, transform_in);
	childNode->AddMesh(std::move(meshPart));
	pRoot->AddChild(std::move(childNode));
}

std::shared_ptr<Material> ModelBuilder::CreateMaterialFromTexture(Graphics& gfx,const char* albedoPath, const char* normalPath, const char* metallicPath, const char* roughnessPath, const char* aoPath)
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

void ModelBuilder::Update(Graphics& gfx, float deltaTime)
{
	Model::Update(gfx, deltaTime);
}

void ModelBuilder::Render(Graphics& gfx)
{
	Model::Render(gfx);
}



