#pragma once
#include "Scene/RenderableObject.h"

class Material
{
public:
	Material(Graphics& g);
	void Update();
	void Bind();
	void SetAmbient(const XMFLOAT4& ambient);
	void SetEmissive(const XMFLOAT4& emissive);
	void SetMetallic(float metallic);
	void SetRoughness(float roughness);
	void SetAO(float AO);
	void controlWindow();
	
private:
	Graphics& m_graphics;
	ConstantBuffer<cb_psMaterialBuffer> materialBuffer;
};

