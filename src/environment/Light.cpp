#include "Light.h"

EnvironmentLight::EnvironmentLight(const std::string& name,Graphics& g,std::shared_ptr<ShaderManager> manager)
	:
	RenderableObject(name,g,std::move( manager )),
	m_name(name),
	m_graphics(g)
{

	PS_Buffer.Initialize(g);
	PS_Buffer.data.light.pointPos = XMFLOAT3(0.0f, 10.0f, 0.0f);
	PS_Buffer.data.light.spotPos = XMFLOAT3(0.0f, 1.0f, 0.0f);
	PS_Buffer.data.light.dir = XMFLOAT3(0.0f, 1.0f, 0.0f);
	PS_Buffer.data.light.range = 1000.0f;
	PS_Buffer.data.light.cone = 20.0f;
	PS_Buffer.data.light.att = XMFLOAT3(0.4f, 0.02f, 0.0f);
	PS_Buffer.data.light.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	PS_Buffer.data.light.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	PS_Buffer.data.light.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

void EnvironmentLight::SetLight(DirectX::XMFLOAT4& color, DirectX::XMFLOAT3& postition)
{

}

void EnvironmentLight::Update(float deltaTime)
{
	RenderableObject::Update(deltaTime);
	PS_Buffer.Update(m_graphics);

}

void EnvironmentLight::Render()
{
	m_graphics.GetDeviceResources()->GetContext()->PSSetConstantBuffers(0, 1, PS_Buffer.GetAddressOf());
	RenderableObject::Render();
}

void EnvironmentLight::controlWindow()
{
}
