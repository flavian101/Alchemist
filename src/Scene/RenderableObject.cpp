#include "RenderableObject.h"
#include "Shaders/ShaderManager.h"
#include "Graphics/Graphics.h"

RenderableObject::RenderableObject(const std::string& name, Graphics& g, std::shared_ptr<ShaderManager> shaderManager)
	:
	m_name(name),
	m_graphics(g),
	m_shaderManager(shaderManager)
{
}
RenderableObject::~RenderableObject()
{
}

void RenderableObject::Update(float time)
{


}
void RenderableObject::Render()
{
	m_shaderManager->BindShaders();
}


std::string RenderableObject::getName() const
{
	return m_name;
}

void RenderableObject::setName(const std::string& name)
{
	this->m_name = name;
}

std::shared_ptr<ShaderManager> RenderableObject::GetShadermanager() const
{
	return m_shaderManager;
}

void RenderableObject::SetShaderManager(std::shared_ptr<ShaderManager> manager)
{
	m_shaderManager = manager;
}

void RenderableObject::controlWindow()
{
	ImGui::Text("Properties");
}


