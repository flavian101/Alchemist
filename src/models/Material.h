#pragma once
#include <unordered_map>
#include "stdafx.h"
#include <string>
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Utilis.h"
#include "Scene/Shaders/ShaderManager.h"

class Graphics;


class Material
{
public:
	enum class TextureType
	{
		Albedo,
		Normal,
		Metallic,
		Roughness,
		AmbientOcclusion,
		Specular
	};
	Material(Graphics& gfx);
	~Material();
	void Update(Graphics& gfx);
	void Bind(Graphics& gfx);
	void UnbindMaterials(Graphics& gfx);

	void LoadTexture(Graphics& gfx,TextureType type, const std::string& path);

	void HasAlbedo(bool has) { hasAlbedoMap = has; };
	void HasBaseColor(bool has) { hasBaseColor = has; }
	void HasNormal(bool has) { hasNormalMap = has; };
	void HasMetallic(bool has) { hasMetallicMap = has; };
	void HasRoughness(bool has) { hasRoughnessMap = has; };
	void HasAO(bool has) { hasAOMap = has; };
	void HasSpecularMap(bool has) { hasSpecularMap = has; }

	void SetAmbient(const XMFLOAT4& ambient);
	void SetEmissive(const XMFLOAT4& emissive);
	void SetMetallic(float metallic);
	void SetRoughness(float roughness);
	void SetAO(float AO);
	
	void controlWindow();
	
private:
	void BindTextures(Graphics& gfx);

	
private:
	std::unique_ptr<Utils::Sampler> samp;
	ConstantBuffer<cb_psMaterialBuffer> materialBuffer;
	std::unordered_map<TextureType, std::unique_ptr<Utils::Texture>> textures;
	std::unique_ptr<ShaderManager> shaderManager;
	bool hasAlbedoMap;
	bool hasBaseColor;
	bool hasNormalMap;
	bool hasMetallicMap;
	bool hasRoughnessMap;
	bool hasAOMap;
	bool hasSpecularMap;
private:
	friend class ModelLoader;
	friend class ModelExporter;
};

