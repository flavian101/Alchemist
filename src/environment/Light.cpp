#include "Light.h"
#include "imgui/imgui.h"
#include "Graphics/Graphics.h"
#include "Scene/Shaders/ShaderManager.h"

Light::Light(const std::string& name,Graphics& g,std::shared_ptr<ShaderManager> manager)
	:
	RenderableObject(name,g,std::move( manager )),
	m_name(name),
	m_graphics(g)
{

	PS_Buffer.Initialize(g);
	PS_Buffer.data.lightStruct.position = XMFLOAT3(0.0f, 10.0f, 0.0f);
	PS_Buffer.data.lightStruct.dir = XMFLOAT3(0.0f, -1.0f, 0.0f);
	PS_Buffer.data.lightStruct.range = 1000.0f;
	PS_Buffer.data.lightStruct.cone = 20.0f;
	PS_Buffer.data.lightStruct.att = XMFLOAT3(0.4f, 0.02f, 0.0f);
	PS_Buffer.data.lightStruct.intensity = 1.0f;
	PS_Buffer.data.lightStruct.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	PS_Buffer.data.lightStruct.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	PS_Buffer.data.lightStruct.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	PS_Buffer.data.lightStruct.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

void Light::SetPosition(const XMFLOAT3& position)
{
	PS_Buffer.data.lightStruct.position = position;
}

void Light::SetColor(const XMFLOAT4& color) 
{
	PS_Buffer.data.lightStruct.color = color;
}

void Light::SetIntensity(float intensity)
{
	PS_Buffer.data.lightStruct.intensity = intensity;
}
void Light::SetDiffuse(XMFLOAT4 diffuse)
{
	PS_Buffer.data.lightStruct.diffuse = diffuse;
}
void Light::SetAmbient(XMFLOAT4 ambient)
{
	PS_Buffer.data.lightStruct.ambient = ambient;
}
void Light::SetSpecular(XMFLOAT4 specular)
{
	PS_Buffer.data.lightStruct.specular = specular;
}
void Light::Update(float deltaTime)
{
	RenderableObject::Update(deltaTime);
	PS_Buffer.Update(m_graphics);
}

void Light::Render()
{
	m_graphics.GetContext()->PSSetConstantBuffers(0, 1, PS_Buffer.GetAddressOf());
	RenderableObject::Render();
}

void Light::controlWindow()
{
	ImGui::Text("Light Settings");
	//Math::ImGuiDragXMVector3("", camPosition);
	ImGui::DragFloat3("Light Direction", &PS_Buffer.data.lightStruct.dir.x);
	ImGui::ColorEdit4("Ambient", &PS_Buffer.data.lightStruct.ambient.x);
	ImGui::ColorEdit4("diffuse", &PS_Buffer.data.lightStruct.diffuse.x);
	ImGui::ColorEdit4("light Color", &PS_Buffer.data.lightStruct.color.x);
	ImGui::DragFloat("Light Intensity", &PS_Buffer.data.lightStruct.intensity);
	//ImGui::PushItemWidth(100);
	//ImGui::InputFloat("speed", &cameraSpeed);
	//ImGui::PopItemWidth();
}
