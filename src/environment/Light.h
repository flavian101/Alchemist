#pragma once
#include "Scene/RenderableObject.h"


class EnvironmentLight: public RenderableObject
{
public:
	EnvironmentLight(const std::string& name, Graphics& g, ShaderManager manager);
	void SetLight(DirectX::XMFLOAT4& color, DirectX::XMFLOAT3& postition);
	void Update(float deltaTime)override;
	void Render()override;
	void controlWindow() override;


private:
	Graphics& m_graphics;
	std::string m_name;
	ConstantBuffer<cb_psConstantBuffer> PS_Buffer;
};

