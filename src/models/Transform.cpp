#include "Transform.h"

Transform::Transform(Graphics& g,const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
	:
	m_graphics(g),
	m_position(XMLoadFloat3(&position)),
	m_rotation(XMLoadFloat3(&rotation)),
	m_scale(XMLoadFloat3(&scale))
{
	CB_Buffer.Initialize(g);
	transform = XMMatrixIdentity();
	//scale translation rotation
	//transform = XMVectorMultiply()
}

//constexpr XMVECTOR Transform::Vectorfromfloat3(XMFLOAT3& float3)
//{
//	return XMVectorSet(float3.x, float3.y, float3.z, 0.0f);;
//}
//
XMMATRIX Transform::GetTransform() const
{
	return transform;
}

void Transform::Update(float time)
{
	// Create the matrices for each component
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(m_scale);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(m_rotation);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(m_position);

	transform = XMMatrixMultiply(scaleMatrix, rotationMatrix);
	transform = XMMatrixMultiply(transform, translationMatrix);

	//update matrix transfoms
	CB_Buffer.data.WVP = XMMatrixTranspose(XMMatrixMultiply(transform, 
		XMMatrixMultiply(
		m_graphics.GetViewMatrix(), m_graphics.GetProjectionMatrix())));
	CB_Buffer.data.Model = XMMatrixTranspose(transform);
	CB_Buffer.Update(m_graphics);
}

void Transform::BindConstantBuffer()
{
	m_graphics.GetContext()->VSSetConstantBuffers(0, 1, CB_Buffer.GetAddressOf());
}
