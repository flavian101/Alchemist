#include "PerspectiveCamera.h"


PerspectiveCamera::PerspectiveCamera(float aspectRatio,
    float nearPlane, float farPlane,
    const DirectX::XMVECTOR& position,
    const DirectX::XMVECTOR& target,
    const DirectX::XMVECTOR& up)
    : Camera()// Set a default FOV of 45 degrees


{
    // Initialize camera position, target, and up vector
    camPosition = position;
    camTarget = target;
    camUp = up;

    this->mode = FreeLook;
    this->m_FOV = 45.0;
    this->m_aspectRatio = aspectRatio;
    this->m_nearPlane = nearPlane;
    this->m_farPlane = farPlane;
    this->cameraSpeed = 2.0f;
    this->distance = 4.0f;
    this->cameraOffsetY = 4.0f;


    // Calculate the initial view matrix
    this->camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
    prevCamPosition = camPosition;
    prevCamTarget = camTarget;
}

void PerspectiveCamera::SetCamera(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
   this->m_FOV = FOV;
   this->m_aspectRatio = aspectRatio;
   this->m_nearPlane = nearPlane;
   this->m_farPlane = farPlane;
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
              // Reset();

            }
            else if (currentItem == ThirdPerson) {
                //Reset();

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
    if (currentCameraMode == ThirdPerson)
    {
        ThirdPersonWindow();

    }
    else if (currentCameraMode == FreeLook)
    {
        FreeLookWindow();

    }

    ImGui::Columns(1);
}

void PerspectiveCamera::ThirdPersonWindow()
{
    ImGui::Text("Third person Settings");
    Math::ImGuiDragXMVector3("target", camTarget);
    Math::ImGuiDragXMVector3("position", camPosition);
    ImGui::PushItemWidth(100);
    ImGui::DragFloat("Distance", &cameraOffsetY,1.0f,0.0f);
    ImGui::DragFloat("camera speed", &cameraSpeed,1.0f,1.0f);
    ImGui::DragFloat("radius", &cameraRadius,1.0f,2.0f,200.0f);
    ImGui::DragFloat("camera lag", &cameraLag,0.01f,0.001f,1.0f);

    ImGui::PopItemWidth();
}

void PerspectiveCamera::FreeLookWindow()
{
    
    ImGui::Text("FreeLook Settings");
    Math::ImGuiDragXMVector3("position" ,camPosition);
    ImGui::PushItemWidth(100);
    ImGui::InputFloat("speed", &cameraSpeed);
    ImGui::PopItemWidth();

}

void PerspectiveCamera::Move(float leftRight, float backForward, float upDown, float speed, float time)
{
    // Calculate the movement vectors based on the given parameters
    DirectX::XMVECTOR moveVec = leftRight * camRight + backForward * camForward + upDown * camUp;

    // Calculate the movement distance based on speed and time
    moveVec *= speed * time;

    // Update the camera position
    camPosition += moveVec;

    // Update the camera target
    camTarget = camPosition + camForward;

    // Update the view matrix
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

void PerspectiveCamera::UpdateCharacterPosition(const DirectX::XMVECTOR& newPosition, const XMVECTOR& newOrientation)
{
    this->characterPosition = newPosition;
    this->characterOrientation = newOrientation;
}

void PerspectiveCamera::UpdateThirdPersonCamera(float delta)
{
    // Calculate the desired camera position and target based on character position and orientation
    XMVECTOR desiredCamPosition = characterPosition - characterOrientation * cameraRadius;
    desiredCamPosition = XMVectorSetY(desiredCamPosition, desiredCamPosition.m128_f32[1] + cameraOffsetY);
    XMVECTOR desiredCamTarget = characterPosition;

    // Smooth the camera movement
    camPosition = XMVectorLerp(prevCamPosition, desiredCamPosition, cameraLag);
    camTarget = XMVectorLerp(prevCamTarget, desiredCamTarget, cameraLag);

    // Calculate the camera's orientation based on the difference between the character's position and the camera's position
    XMVECTOR camForward = XMVector3Normalize(desiredCamPosition - characterPosition);
    XMVECTOR camRight = XMVector3Normalize(XMVector3Cross(camUp, camForward));
    XMVECTOR camUp = XMVector3Normalize(XMVector3Cross(camForward, camRight));

    // Update the view matrix
    camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

    // Store the current camera position and target for the next frame
    prevCamPosition = camPosition;
    prevCamTarget = camTarget;
}

void PerspectiveCamera::ThirdPersonCamera(float delta)
{
    // Calculate the rotation matrix based on pitch and yaw
    DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(-camPitch, camYaw, 0);

    // Derive the forward vector from the rotation matrix
    DirectX::XMVECTOR camForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f); // Default forward vector in the +Z direction
    camForward = DirectX::XMVector3TransformNormal(camForward, camRotationMatrix);
    camForward = DirectX::XMVector3Normalize(camForward);

    // Derive the right vector from the forward vector (cross product with world up vector)
    DirectX::XMVECTOR camRight = DirectX::XMVector3Cross(camForward, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)); // World up vector is (0, 1, 0)
    camRight = DirectX::XMVector3Normalize(camRight);

    // Update camera target based on moveBackForward and moveLeftRight
    camTarget += camForward * moveBackForward + camRight * moveLeftRight;

    // Calculate the new camera position based on the transformed forward vector and distance from the target
    DirectX::XMVECTOR newCamPosition = DirectX::XMVectorAdd(camTarget, DirectX::XMVectorScale(camForward, -distance)); // Move along negative forward vector

    // Set the camera position
    camPosition = newCamPosition;

    // Update the camera view matrix
    camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)); // Set camUp to world up direction

    // Reset movement values after update (for a more responsive feel)
    moveBackForward = 0.0f;
    moveLeftRight = 0.0f;
}

void PerspectiveCamera::Reset()
{

    camPosition = GetPos();
    camTarget = GetTarget(); // Default target at (0, 0, 0)
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
