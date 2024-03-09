#include "Camera.h"

Camera::Camera(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& target, const DirectX::XMVECTOR& up)
    : camPosition(position), camTarget(target), camUp(up)
{
    UpdateCamera();
}

void Camera::UpdateCamera()
{
    using namespace DirectX;

    // Calculate rotation matrix
    camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);

    // Calculate new target direction
    camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
    camTarget = XMVector3Normalize(camTarget);

    // Calculate new right and up vectors
    camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
    camUp = XMVector3TransformCoord(camUp, camRotationMatrix);

    // Calculate new position based on movement
    XMVECTOR moveDirection = moveLeftRight * camRight + moveBackForward * camTarget;
    camPosition += moveDirection;

    // Reset movement variables
    moveLeftRight = 0.0f;
    moveBackForward = 0.0f;

    // Update the view matrix
    camView = XMMatrixLookAtLH(camPosition, camPosition + camTarget, camUp);
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
    return camPosition + camTarget;
}

void Camera::SetPosition(const DirectX::XMVECTOR& position)
{
    camPosition = position;
}

void Camera::SetTarget(const DirectX::XMVECTOR& target)
{
    camTarget = XMVector3Normalize(target - camPosition);
}