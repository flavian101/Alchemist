#pragma once
#include "Scene/RenderableObject.h"


class Light: public RenderableObject
{
public:
	Light(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> manager);
	virtual void SetPosition(const XMFLOAT3& position);
	virtual void SetColor(const XMFLOAT4& color);
	virtual void SetIntensity(float intensity);
	virtual void SetDiffuse(XMFLOAT4 diffuse);
	virtual void SetAmbient(XMFLOAT4 ambient);
	virtual void SetSpecular(XMFLOAT4 specular);
	virtual void Update(float deltaTime)override;
	virtual void Render()override;
	virtual void controlWindow() override;


private:
	Graphics& m_graphics;
	std::string m_name;
	ConstantBuffer<cb_psConstantBuffer> PS_Buffer;
};

