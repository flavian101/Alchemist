#include "Camera.h"

Camera::Camera(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& target, const DirectX::XMVECTOR& up)
    : camPosition(position), camTarget(target), camUp(up)
{
    camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

void Camera::UpdateCamera()
{
	using namespace DirectX;
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	camPosition += moveLeftRight * camRight;
	camPosition += moveBackForward * camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

DirectX::XMMATRIX Camera::GetView() const
{
    return camView;
}

DirectX::XMVECTOR Camera::GetPos() const
{
    return camPosition;
}

DirectX::XMVECTOR Camera::GetTarget() const
{   
	return camTarget;
}

void Camera::SetPosition(const DirectX::XMVECTOR& position)
{
    camPosition = position;
}

void Camera::SetTarget(const DirectX::XMVECTOR& target)
{
    camTarget = XMVector3Normalize(target - camPosition);
}