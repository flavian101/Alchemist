#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(float width, float height)
	:Camera(m_aspectRatio, m_nearPlane, m_farPlane, camPosition, camTarget, camUp)
{
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
