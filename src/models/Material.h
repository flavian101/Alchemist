#pragma once
#include "Scene/RenderableObject.h"
#include <unordered_map>

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

	void SetAmbient(const XMFLOAT4& ambient);
	void SetEmissive(const XMFLOAT4& emissive);
	void SetMetallic(float metallic);
	void SetRoughness(float roughness);
	void SetAO(float AO);
	void controlWindow();
	bool hasAlbedoMap    = false;
	bool hasNormalMap    = false;
	bool hasMetallicMap  = false;
	bool hasRoughnessMap = false;
	bool hasAOMap        = false;
private:
	void BindTextures();

	
private:
	Graphics& m_graphics;
	std::unique_ptr<Utils::Sampler> samp;
	ConstantBuffer<cb_psMaterialBuffer> materialBuffer;
	std::unordered_map<TextureType, std::unique_ptr<Utils::Texture>> textures;
};

