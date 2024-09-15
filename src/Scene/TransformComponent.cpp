#include "TransformComponent.h"

TransformComponent::TransformComponent(Graphics& g)
	:
	m_transform(g)
{}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Update(float deltaTime)
{
	m_transform.Update(deltaTime);

}

void TransformComponent::Bind()
{
	m_transform.BindConstantBuffer();

}

void TransformComponent::ControlWindow()
{
	m_transform.controlWindow();

}

XMMATRIX TransformComponent::GetTransform()const
{
	return m_transform.GetTransform();
}

XMVECTOR TransformComponent::GetTranslation() const
{
	return m_transform.GetPosition();
}

XMVECTOR TransformComponent::GetRotation() const
{
	return m_transform.GetRotation();
}

XMVECTOR TransformComponent::GetScale()const
{
	return m_transform.GetScale();;
}

void TransformComponent::setTranslation(const XMFLOAT3& translation)
{
	m_transform.SetPosition(translation);
}

void TransformComponent::setRotation(const XMFLOAT4& rotation)
{
	m_transform.SetRotation(rotation);
}

void TransformComponent::setScale(const XMFLOAT3& scale)
{
	m_transform.SetScale(scale);
}

