#include "ThirdPerson.h"

ThirdPerson::ThirdPerson()
    :
    distance(20.0f),
    height(5.0f),
    cameraLag(0.1f),
    playerPosition(camPosition),
    playerOrientation(XMQuaternionIdentity()),
    FollowPlayer(false),
    rotationSpeed(0.3f),
    moveSpeed(10.0f)
{
    mode = thirdPerson;

}

void ThirdPerson::SetThirdPerson(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
    m_FOV = FOV;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

void ThirdPerson::SetPlayerPosition(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& orientation)
{
    playerPosition = position;
    playerOrientation = orientation;
}

void ThirdPerson::Update(float deltaTime) {
    mode = thirdPerson;
    PerspectiveCamera::Update(deltaTime);

    XMVECTOR targetPosition = playerPosition;
    XMMATRIX rotationMatrix;
    if (FollowPlayer)
    {
        rotationMatrix = XMMatrixRotationQuaternion(playerOrientation);
    }
    else
    {
        rotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0.0f); 
    }
    XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); 
    XMVECTOR transformedForward = XMVector3TransformCoord(defaultForward, rotationMatrix);

    XMVECTOR desiredPosition = targetPosition - transformedForward * distance + XMVectorSet(0.0f, height, 0.0f, 0.0f);

    XMVECTOR currentPosition = GetPos();
    XMVECTOR smoothedPosition = XMVectorLerp(currentPosition, desiredPosition, cameraLag);

    SetPosition(smoothedPosition);
    camView = XMMatrixLookAtLH(smoothedPosition, targetPosition, camUp);
}

void ThirdPerson::ControlWindow()
{
    PerspectiveCamera::ControlWindow();
	ImGui::Text("Third person Settings");
    ImGui::PushItemWidth(100);
    ImGui::Checkbox("Follow Player", &FollowPlayer);
    ImGui::DragFloat("moveSpeed", &moveSpeed, 1.0f, 0.0f);
    ImGui::DragFloat("rotation Speed", &rotationSpeed, 0.01f, 0.001f);
    ImGui::DragFloat("Distance", &distance,1.0f,0.0f);
    ImGui::DragFloat("height", &height,1.0f,1.0f);
    ImGui::DragFloat("camera lag", &cameraLag,0.01f,0.001f,1.0f);
    ImGui::PopItemWidth();


}

void ThirdPerson::AddYaw(float amount)
{
    camYaw += amount;
}

void ThirdPerson::AddPitch(float amount)
{
    camPitch += amount;
    camPitch = std::max(-DirectX::XM_PIDIV2 + 0.1f, std::min(DirectX::XM_PIDIV2 - 0.1f, camPitch));

}

XMVECTOR ThirdPerson::GetForwardDirection() const
{
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0.0f);
    XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
    return XMVector3TransformCoord(defaultForward, rotationMatrix);
}

XMVECTOR ThirdPerson::GetRightDirection() const
{
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0.0f);
    XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    return XMVector3TransformCoord(defaultRight, rotationMatrix);
}
