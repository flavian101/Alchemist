#include "RenderableObject.h"

RenderableObject::RenderableObject(const std::string& name,Graphics& g,ShaderManager shaderManager)
	:
	m_name(name),
	m_graphics(g),
	m_shaderManager(shaderManager),
	m_transform(g)
{}

RenderableObject::~RenderableObject()
{
}

void RenderableObject::Update(float time)
{
	m_transform.Update(time);

}

void RenderableObject::Render()
{
	m_transform.BindConstantBuffer();
	m_shaderManager.BindShaders();
	

}

XMMATRIX RenderableObject::GetTransform()const 
{
	return m_transform.GetTransform();
}

XMVECTOR RenderableObject::GetTranslation() const
{
	return m_transform.GetPosition();
}

XMVECTOR RenderableObject::GetRotation() const
{
	return m_transform.GetRotation();
}

XMVECTOR RenderableObject::GetScale()const
{
	return m_transform.GetScale();;
}

void RenderableObject::setTranslation(const XMFLOAT3& translation)
{
	m_transform.SetPosition(translation);
}

void RenderableObject::setRotation(const XMFLOAT4& rotation)
{
	m_transform.SetRotation(rotation);
}

void RenderableObject::setScale(const XMFLOAT3& scale)
{
	m_transform.SetScale(scale);
}

std::string RenderableObject::getName() const
{
	return m_name;
}

void RenderableObject::setName(const std::string& name)
{
	this->m_name = name;
}

ShaderManager RenderableObject::GetShadermanager() const
{
	return m_shaderManager;
}

void RenderableObject::controlWindow()
{
	ImGui::Text("Properties");
	m_transform.controlWindow();
}


