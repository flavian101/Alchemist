#include "Material.h"
#include "Graphics/Graphics.h"
#include <imgui/imgui.h>


Material::Material(Graphics& gfx)
	:
	hasAlbedoMap(FALSE),
	hasNormalMap (FALSE),
	hasMetallicMap( FALSE),
	hasRoughnessMap (FALSE),
	hasAOMap (FALSE)
{
	materialBuffer.Initialize(gfx);
	materialBuffer.data.materialStruct.baseColor = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	materialBuffer.data.materialStruct.emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	materialBuffer.data.materialStruct.metallic =0.5f;
	materialBuffer.data.materialStruct.roughness = 0.5f;
	materialBuffer.data.materialStruct.ao = 1.0f;
	samp = std::make_unique<Utils::Sampler>(gfx);
}

void Material::Update(Graphics& gfx)
{
	materialBuffer.data.materialStruct.hasAlbedoMap =static_cast<BOOL>(hasAlbedoMap);
	materialBuffer.data.materialStruct.hasNormalMap = static_cast<BOOL>(hasNormalMap);
	materialBuffer.data.materialStruct.hasMetallicMap = static_cast<BOOL>(hasMetallicMap);
	materialBuffer.data.materialStruct.hasRoughnessMap = static_cast<BOOL>(hasRoughnessMap);
	materialBuffer.data.materialStruct.hasAOMap = static_cast<BOOL>(hasAOMap);
	materialBuffer.Update(gfx);
}

void Material::Bind(Graphics& gfx)
{
	gfx.GetContext()->PSSetConstantBuffers(1, 1, materialBuffer.GetAddressOf());
	BindTextures(gfx);
	samp->Bind(gfx);
}

void Material::LoadTexture(Graphics& gfx,TextureType type, const std::string& path)
{
	if (path.empty())
	{
		return;
	}
	auto texture = std::make_unique<Utils::Texture>(gfx, path.c_str());
	textures[type] = std::move(texture);
}

void Material::BindTextures(Graphics& gfx)
{
	for (const auto& [type, texture] : textures)
	{
		if (texture)
		{
			texture->Bind(gfx,static_cast<UINT>(type)); 
		}
	}
}

void Material::SetAmbient(const XMFLOAT4& baseColor)
{
	materialBuffer.data.materialStruct.baseColor = baseColor;
}

void Material::SetEmissive(const XMFLOAT4& emissive)
{
	materialBuffer.data.materialStruct.emissive = emissive;
}

void Material::SetMetallic(float metallic)
{
	materialBuffer.data.materialStruct.metallic = metallic;
}

void Material::SetRoughness(float roughness)
{
	materialBuffer.data.materialStruct.roughness = roughness;
}
void Material::SetAO(float AO)
{
	materialBuffer.data.materialStruct.ao = AO;
}


void Material::controlWindow()
{
	ImGui::Text("Material settings");
	ImGui::ColorEdit4("Base color", &materialBuffer.data.materialStruct.baseColor.x);
	ImGui::ColorEdit4("emissive", &materialBuffer.data.materialStruct.emissive.x);
	ImGui::DragFloat("Metallic", &materialBuffer.data.materialStruct.metallic);
	ImGui::DragFloat("Roughness", &materialBuffer.data.materialStruct.roughness);
	ImGui::DragFloat("AO", &materialBuffer.data.materialStruct.ao, 0.2f);
	ImGui::Text("Textures material activation");
	// Toggle texture maps
	ImGui::Checkbox("Albedo Map", &hasAlbedoMap);
	ImGui::Checkbox("Normal Map", &hasNormalMap);
	ImGui::Checkbox("Metallic Map", &hasMetallicMap);
	ImGui::Checkbox("Roughness Map", &hasRoughnessMap);
	ImGui::Checkbox("AO Map", &hasAOMap);
	
	//ImGui::Checkbox("metallic map", &materialBuffer.data.materialStruct.hasMetallicMap);
	//ImGui::Checkbox("rougnes map", &materialBuffer.data.materialStruct.hasRoughnessMap);
	//ImGui::Checkbox("AO map", &materialBuffer.data.materialStruct.hasAOMap);
}
