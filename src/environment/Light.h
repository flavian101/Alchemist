#pragma once
#include "Graphics/ConstantBuffer.h"
#include "Scene/GameObject.h"

class Graphics;
class ShaderManager;
class Transform;

class Light :public GameObject
{
public:
	Light(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> manager);
	virtual void SetPosition(const XMFLOAT3& position);
	virtual void SetColor(const XMFLOAT4& color);
	virtual void SetIntensity(float intensity);
	virtual void SetDiffuse(XMFLOAT4 diffuse);
	virtual void SetAmbient(XMFLOAT4 ambient);
	virtual void SetSpecular(XMFLOAT4 specular);
	virtual void Update(Graphics& gfx,float deltaTime)override;
	virtual void Render(Graphics& gfx)override;
	virtual void controlWindow(Graphics& gfx);


private:
	std::string m_name;
	ConstantBuffer<cb_psConstantBuffer> PS_Buffer;

};

