#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(float width, float height,
	const DirectX::XMVECTOR& position,
	const DirectX::XMVECTOR& target,
	const DirectX::XMVECTOR& up
	)
	: Camera(),
	m_width(width),
	m_height(height)
	
{
	m_nearPlane = 0.0f,  // Initialize near plane
	m_farPlane = 1.0f;    // Initialize far plane
	// Initialize camera position, target, and up vector
	camPosition = position;
	camTarget = target;
	camUp = up;

	camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);

}

void OrthographicCamera::SetCamera(float width, float height, float nearZ, float farZ)
{
	m_width = width;
	m_height = height;
	m_nearPlane = nearZ;
	m_farPlane = farZ;

}

DirectX::XMMATRIX OrthographicCamera::GetProjectionMatrix() const
{
	return XMMatrixOrthographicLH(m_width, m_height, m_nearPlane, m_farPlane);
}

void OrthographicCamera::Update(float delta)
{
}


DirectX::XMMATRIX OrthographicCamera::GetView() const
{
	return camView;
}

DirectX::XMVECTOR OrthographicCamera::GetPos() const
{
	return camPosition;
}

DirectX::XMVECTOR OrthographicCamera::GetTarget() const
{
	return camTarget;
}