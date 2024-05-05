#include "RenderableObject.h"

RenderableObject::RenderableObject(const std::string& name,Graphics& g,ShaderManager shaderManager)
	:
	m_name(name),
	m_graphics(g),
	m_shaderManager(shaderManager),
	m_transform(g)
{
	objectTransform = XMMatrixIdentity();
	m_position = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);;
	m_rotation = XMQuaternionIdentity();
	m_scale = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
}

RenderableObject::~RenderableObject()
{
}

void RenderableObject::Update(float time)
{
	initializeTransform();
	m_transform.SetTransform(objectTransform);
	m_transform.Update(time);

}

void RenderableObject::Render()
{
	m_transform.BindConstantBuffer();
	m_shaderManager.BindShaders();
	

}

void RenderableObject::initializeTransform()
{
	// Create transformation matrices for rotation, translation, and scaling
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(m_position);
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(m_scale);

	//rotation 
	XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(
		XMVectorGetX(m_rotation),
		XMVectorGetY(m_rotation),
		XMVectorGetZ(m_rotation));

	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotationQuaternion);


	objectTransform = scaleMatrix * rotationMatrix * translationMatrix;

}

XMMATRIX RenderableObject::GetTransform()const 
{
	return objectTransform;
}

void RenderableObject::SetTransform(XMMATRIX transform)
{
	objectTransform = transform;

}
XMVECTOR RenderableObject::GetTranslation() const
{
	return m_position;
}

XMVECTOR RenderableObject::GetRotation() const
{
	return m_rotation;
}

XMVECTOR RenderableObject::GetScale()const
{
	return m_scale;
}

void RenderableObject::setTranslation(const XMFLOAT3& translation)
{
	m_position = XMLoadFloat3(&translation);
}

void RenderableObject::setRotation(const XMFLOAT4& rotation)
{
	m_rotation = XMLoadFloat4(&rotation);
}

void RenderableObject::setScale(const XMFLOAT3& scale)
{
	m_scale = XMLoadFloat3(&scale);
 }

std::string RenderableObject::getName() const
{
	return m_name;
}

void RenderableObject::setName(const std::string& name)
{
	this->m_name = name;
}

void RenderableObject::SetParent(RenderableObject* parent)
{
	m_parent = parent;
}

ShaderManager RenderableObject::GetShadermanager() const
{
	return m_shaderManager;
}

void RenderableObject::controlWindow()
{
	ImGui::Text("Properties");

}


