#include "Transform.h"
#include <MathUtils\MathUtils.h>
#include <DirectXMath.h>
#include "Graphics\Graphics.h"

Transform::Transform(Graphics& g,const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& scale)
	:
	m_graphics(g),
	m_position(XMLoadFloat3(&position)),
	m_rotation(XMLoadFloat4(&rotation)),
	m_scale(XMLoadFloat3(&scale))
{
	CB_Buffer.Initialize(g);
	m_transform = XMMatrixIdentity();
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
	return m_transform;
}

void Transform::setTransform(const FXMMATRIX& matrix)
{
	m_transform = matrix;
}

void Transform::Update(float time)
{
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(m_scale);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(m_rotation);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(m_position);

	m_transform = XMMatrixMultiply(scaleMatrix, rotationMatrix);
	m_transform = XMMatrixMultiply(m_transform, translationMatrix);

	//update matrix transfoms
	CB_Buffer.data.WVP = XMMatrixTranspose(XMMatrixMultiply(m_transform, 
		XMMatrixMultiply(
		m_graphics.GetViewMatrix(), m_graphics.GetProjectionMatrix())));
	CB_Buffer.data.View = XMMatrixTranspose(m_graphics.GetViewMatrix());
	CB_Buffer.data.Model = XMMatrixTranspose(m_transform);
	CB_Buffer.Update(m_graphics);
}

void Transform::UpdateFromTransform(float Time)
{
	CB_Buffer.data.WVP = XMMatrixTranspose(
		XMMatrixMultiply(
			XMMatrixMultiply(m_transform, m_graphics.GetViewMatrix()),
			m_graphics.GetProjectionMatrix()
		));
	CB_Buffer.data.View = XMMatrixTranspose(m_graphics.GetViewMatrix());
	CB_Buffer.data.Model = XMMatrixTranspose(m_transform);
	CB_Buffer.Update(m_graphics);
}

void Transform::BindConstantBuffer()
{
	m_graphics.GetContext()->VSSetConstantBuffers(0, 1, CB_Buffer.GetAddressOf());
}

void Transform::controlWindow()
{
	Math::ImGuiDragXMVector3("position", m_position);
	Math::ImGuiDragXMVector3("Rotation", m_rotation);
	Math::ImGuiDragXMVector3("Scale", m_scale);
}

void Transform::PrintMatrix(const char* name, const XMMATRIX& matrix)
{
	XMFLOAT4X4 matValues;
	XMStoreFloat4x4(&matValues, matrix);  // Store the matrix in a float4x4 structure

	std::cout << name << " Matrix:\n";
	for (int i = 0; i < 4; ++i) {
		std::cout << matValues.m[i][0] << ", "
			<< matValues.m[i][1] << ", "
			<< matValues.m[i][2] << ", "
			<< matValues.m[i][3] << "\n";
	}
	std::cout << std::endl;
}
