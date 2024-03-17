#include "RenderableObject.h"

RenderableObject::RenderableObject(Graphics& g,ShaderManager shaderManager)
	:
	m_graphics(g),
	m_shaderManager(shaderManager),
	m_transform(g)
{
	
}

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

XMMATRIX RenderableObject::GetTransform()
{
	return m_transform.GetTransform();
}

void RenderableObject::setTranslation(XMFLOAT3& translation)
{
	m_transform.SetPosition(translation);
}

void RenderableObject::setRotation(XMFLOAT3& rotation)
{
	m_transform.SetRotation(rotation);
}

void RenderableObject::setScale(XMFLOAT3& scale)
{
	m_transform.SetScale(scale);
}

ShaderManager RenderableObject::GetShadermanager()
{
	return m_shaderManager;
}



