#pragma once
#include <unordered_map>
#include "stdafx.h"
#include <string>
#include "Graphics/ConstantBuffer.h"
#include "Graphics/Utilis.h"

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
		AmbientOcclusion
	};
	Material(Graphics& g);
	void Update();
	void Bind();
	void LoadTexture(TextureType type, const std::string& path);

	void HasAlbedo(bool has) { hasAlbedoMap = has; };
	void HasNormal(bool has) { hasNormalMap = has; };
	void HasMetallic(bool has) { hasMetallicMap = has; };
	void HasRoughness(bool has) { hasRoughnessMap = has; };
	void HasAO(bool has) { hasAOMap = has; };
	void SetAmbient(const XMFLOAT4& ambient);
	void SetEmissive(const XMFLOAT4& emissive);
	void SetMetallic(float metallic);
	void SetRoughness(float roughness);
	void SetAO(float AO);
	void controlWindow();
	
private:
	void BindTextures();

	
private:
	Graphics& m_graphics;
	std::unique_ptr<Utils::Sampler> samp;
	ConstantBuffer<cb_psMaterialBuffer> materialBuffer;
	std::unordered_map<TextureType, std::unique_ptr<Utils::Texture>> textures;
	bool hasAlbedoMap;
	bool hasNormalMap;
	bool hasMetallicMap;
	bool hasRoughnessMap;
	bool hasAOMap;
};

