#include "Transform.h"
#include <MathUtils\MathUtils.h>
#include <DirectXMath.h>
#include "Graphics\Graphics.h"

Transform::Transform(Graphics& gfx,const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& scale)
	:
	m_position(XMLoadFloat3(&position)),
	m_rotation(XMLoadFloat4(&rotation)),
	m_scale(XMLoadFloat3(&scale))
{
	CB_Buffer.Initialize(gfx);
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

void Transform::setTransform(const XMMATRIX& matrix)
{
	m_transform = matrix;
}

void Transform::Update(Graphics& gfx)
{
	XMMATRIX scaleMatrix = XMMatrixScalingFromVector(m_scale);
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(m_rotation);
	XMMATRIX translationMatrix = XMMatrixTranslationFromVector(m_position);

	m_transform = XMMatrixMultiply(scaleMatrix, rotationMatrix);
	m_transform = XMMatrixMultiply(m_transform, translationMatrix);

	//update matrix transfoms

	CB_Buffer.data.modelView = XMMatrixTranspose(gfx.GetViewMatrix());
	CB_Buffer.data.modelViewProj = XMMatrixTranspose(XMMatrixMultiply(m_transform,
		XMMatrixMultiply(
			gfx.GetViewMatrix(), gfx.GetProjectionMatrix())));	CB_Buffer.Update(gfx);
}

void Transform::UpdateFromTransform(Graphics& gfx,FXMMATRIX transform)
{
	CB_Buffer.data.modelView = XMMatrixTranspose(XMMatrixMultiply(transform,gfx.GetViewMatrix()));
	CB_Buffer.data.modelViewProj = XMMatrixTranspose(transform * gfx.GetViewMatrix() * gfx.GetProjectionMatrix());
	CB_Buffer.Update(gfx);
}

void Transform::BindConstantBuffer(Graphics& gfx)
{
	gfx.GetContext()->VSSetConstantBuffers(0, 1, CB_Buffer.GetAddressOf());
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
