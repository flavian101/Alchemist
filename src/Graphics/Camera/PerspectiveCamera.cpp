#include "PerspectiveCamera.h"


PerspectiveCamera::PerspectiveCamera(float aspectRatio,
    float nearPlane, float farPlane,
    const DirectX::XMVECTOR& position,
    const DirectX::XMVECTOR& target,
    const DirectX::XMVECTOR& up)
    : Camera(), m_FOV(XM_PI / 4.0f) // Set a default FOV of 45 degrees


{
    // Initialize camera position, target, and up vector
    camPosition = position;
    camTarget = target;
    camUp = up;


    // Calculate the initial view matrix
    camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

void PerspectiveCamera::SetCamera(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
    m_FOV = FOV;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
}

DirectX::XMMATRIX PerspectiveCamera::GetProjectionMatrix() const
{
   // return DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 100.0f);
    return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_FOV), m_aspectRatio, m_nearPlane, m_farPlane);
}

void PerspectiveCamera::Update(float delta)
{
}

void PerspectiveCamera::ControlWindow()
{

    ImGui::Text("Camera Settings");
    ImGui::Separator();
    ImGui::PushItemWidth(100.0f);
    ImGui::SliderFloat("FOV", &m_FOV, 30.0f, 120.0f);
    ImGui::SliderFloat("Near Plane", &m_nearPlane, 0.1f, 2.0f);
    ImGui::SliderFloat("Far Plane", &m_farPlane, 10.0f, 1000.0f);
    ImGui::PopItemWidth();
    ImGui::Separator();

    ImGui::Text("Camera Mode");
    static CameraMode currentCameraMode = mode; // Initialize with the current mode
    const char* items[] = { "Free Look", "Third Person" };
    int currentItem = static_cast<int>(currentCameraMode);
    // Render the combo box
    if (ImGui::BeginCombo("##CameraModeCombo", items[currentItem])) {
        for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
            bool isSelected = (currentItem == i);
            if (ImGui::Selectable(items[i], isSelected)) {
                currentItem = i;
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();

        // Reset camera position and target when switching modes
        if (currentCameraMode != static_cast<CameraMode>(currentItem)) {
            if (currentItem == FreeLook) {
                Reset();
            }
            else if (currentItem == ThirdPerson) {
                Reset();
            }
            // Recalculate view matrix
            camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
        }
    }

    // Update the current camera mode based on the selected item
    currentCameraMode = static_cast<CameraMode>(currentItem);

    // Update the camera mode of the PerspectiveCamera instance
    mode = currentCameraMode;

    // Call ThirdPersonWindow if the current mode is ThirdPerson
    if (currentCameraMode == ThirdPerson) {
        ThirdPersonWindow();
    }

    ImGui::Columns(1);
}

void PerspectiveCamera::ThirdPersonWindow()
{
    ImGui::Text("Third person Settings");
    ImGui::PushItemWidth(100);
    ImGui::InputFloat("Distance", &distance);
    ImGui::InputFloat("Rotation speed", &cameraSpeed);

    ImGui::PopItemWidth();
}

DirectX::XMVECTOR PerspectiveCamera::GetPos() const
{
    return camPosition;
}

DirectX::XMVECTOR PerspectiveCamera::GetTarget() const
{
    return camTarget;
}

DirectX::XMVECTOR PerspectiveCamera::GetUp() const
{
    return camUp;
}

void PerspectiveCamera::SetPosition(DirectX::XMVECTOR& position)
{
    camPosition = position;

}

void PerspectiveCamera::SetTarget(DirectX::XMVECTOR& target)
{
    camTarget = target;
}

void PerspectiveCamera::SetUP(DirectX::XMVECTOR& up)
{
    camUp = up;
}
// In the Move function
void PerspectiveCamera::Move(float leftRight, float backForward, float upDown, float speed, float time)
{
    // Calculate movement vectors based on camera orientation
    DirectX::XMVECTOR moveVec = moveLeftRight * camRight + moveBackForward * camForward + moveUpDown * camUp;

    // Calculate movement distance based on speed and time
    moveVec *= speed * time;

    // Update camera position
    camPosition += moveVec;

    // Update camera target
    camTarget = camPosition + camForward;

    // Update view matrix
    camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

// In the RotateYaw function
void PerspectiveCamera::RotateYaw(float amount)
{
    // Update camera yaw
    camYaw += amount;

    // Recalculate camera forward and right vectors
    camForward = DirectX::XMVectorSet(
        sinf(camYaw) * cosf(camPitch),
        sinf(camPitch),
        cosf(camYaw) * cosf(camPitch),
        0.0f
    );

    camRight = DirectX::XMVector3Cross(camUp, camForward);

    // Normalize vectors
    camForward = DirectX::XMVector3Normalize(camForward);
    camRight = DirectX::XMVector3Normalize(camRight);

    // Update camera target based on the new position and direction
    camTarget = camPosition + camForward;

    // Update view matrix
    camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

// In the RotatePitch function
void PerspectiveCamera::RotatePitch(float amount)
{
    // Update camera pitch
    camPitch += amount;

    // Clamp pitch to avoid gimbal lock
    camPitch = std::max(-DirectX::XM_PIDIV2 + 0.1f, std::min(DirectX::XM_PIDIV2 - 0.1f, camPitch));

    // Recalculate camera forward vector
    camForward = DirectX::XMVectorSet(
        sinf(camYaw) * cosf(camPitch),
        sinf(camPitch),
        cosf(camYaw) * cosf(camPitch),
        0.0f
    );

    // Update camera target based on the new position and direction
    camTarget = camPosition + camForward;

    // Update view matrix
    camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

void PerspectiveCamera::SetPitch(float pitch)
{
    camPitch = pitch;
}



DirectX::XMMATRIX PerspectiveCamera::GetView() const
{
    return camView;
}

void PerspectiveCamera::FreeLookCamera(float delta)
{
    Move(moveLeftRight, moveBackForward, moveUpDown, cameraSpeed, delta);
    moveLeftRight = 0.0f;
    moveBackForward = 0.0f;
    moveUpDown = 0.0f;
}

void PerspectiveCamera::FirstPersonCamera(float delta)
{
}

void PerspectiveCamera::ThirdPersonCamera(float delta)
{
    camTarget = XMVectorSetY(camTarget, XMVectorGetY(camTarget));
    camRotationMatrix = XMMatrixRotationRollPitchYaw(-camPitch, camYaw, 0);
    camPosition = XMVector3TransformNormal(DefaultForward, camRotationMatrix);

    camPosition = (camPosition * distance) + camTarget;

    camForward = XMVector3Normalize(camTarget - camPosition);
    camForward = XMVectorSetY(camForward, 0.0f);
    camForward = XMVector3Normalize(camForward);

    camRight = XMVectorSet(-XMVectorGetZ(camForward), 0.0f, XMVectorGetX(camForward), 0.0f);
    camUp = XMVector3Normalize(XMVector3Cross(XMVector3Normalize(camPosition - camTarget), camRight));
  
    camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

}

void PerspectiveCamera::Reset()
{

    camPosition = DirectX::XMVectorSet(0.0f, 5.0f, -10.0f, 0.0f);
    camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // Default target at (0, 0, 0)
    camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}


void PerspectiveCamera::SetView(DirectX::XMMATRIX view)
{
    camView = view;
}

float PerspectiveCamera::GetFOV() const
{
    return m_FOV;
}

void PerspectiveCamera::setFOV(float fov)
{
    m_FOV = fov;
}
