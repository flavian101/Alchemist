#include "GameObject.h"
#include "Shaders/ShaderManager.h"
#include "Graphics/Graphics.h"
#include "models/Model.h"

GameObject::GameObject(const std::string& name)
	:
	m_name(name)
{}
GameObject::~GameObject()
{
}

void GameObject::Update(Graphics& gfx,float time)
{
}
void GameObject::Render(Graphics& gfx)
{
}
std::string GameObject::GetName() const
{
	return m_name;
}

void GameObject::setName(const std::string& name)
{
	this->m_name = name;
}



void GameObject::controlWindow(Graphics& gfx)
{
	ImGui::Text("Properties");
}
//
//void GameObject::Move(const DirectX::XMVECTOR& direction, float speed, float deltaTime)
//{
//	m_position += direction * speed * deltaTime;
//	//setTranslation(Math::XMVectorToFloat3(m_position));
//}
//
//void GameObject::Rotate(float yawAmount, float pitchAmount)
//{
//	yaw += yawAmount;
//	pitch += pitchAmount;
//	// Clamp pitch to avoid gimbal lock
//	//pitch = std::max(-XM_PIDIV2 + 0.1f, std::min(XM_PIDIV2 - 0.1f, pitch));
//
//	// Create a quaternion based on the yaw and pitch
//	DirectX::XMVECTOR yawQuat = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), yaw);
//	DirectX::XMVECTOR pitchQuat = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), pitch);
//
//	// Combine the yaw and pitch quaternions
//	m_orientation = DirectX::XMQuaternionMultiply(pitchQuat, yawQuat);
//	m_orientation = DirectX::XMQuaternionNormalize(m_orientation);
//	//setRotation(Math::XMVectorToFloat4(m_orientation));
//}




