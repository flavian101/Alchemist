#include "Camera.h"

Camera::Camera(float aspectRatio, float nearPlane, float farPlane, const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& target, const DirectX::XMVECTOR& up)
    : camPosition(position), camTarget(target), camUp(up)
{}


void Camera::SetCameraMode(CameraMode mode)
{
    cameraMode = mode;
    // Depending on the mode, call the corresponding camera mode function
    switch (cameraMode)
    {
    case CameraMode::FreeLook:
       // FreeLook();
        break;
    case CameraMode::ThirdPerson:
       // ThirdPersonCamera(/* pass appropriate parameters */);
        break;
        // Add additional modes as needed
    }
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