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
    this->cameraOffsetY = 6.0f;


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
    ImGui::SliderFloat("Far Plane", &m_farPlane, 10.0f, 10000.0f);
    ImGui::PopItemWidth();
    ImGui::Separator();

    ImGui::Text("Camera Mode");
    const char* items[] = { "Free Look", "Third Person" };
    int currentItem = static_cast<int>(mode);
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

        
    }
    mode = static_cast<CameraMode>(currentItem);
    if (mode == ThirdPerson) {
        ThirdPersonWindow();
    }
    else if (mode == FreeLook) {
        FreeLookWindow();
    }


    ImGui::Columns(1);
}

void PerspectiveCamera::ThirdPersonWindow()
{
    ImGui::Text("Third person Settings");
    ImGui::Text("delta %.3f ",time);

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
    time = delta;
    // Convert the character's orientation to a rotation matrix
    XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(characterOrientation);

    // Calculate the desired camera position
    XMVECTOR cameraOffset = XMVectorSet(0.0f, cameraOffsetY, -cameraRadius, 0.0f);

    XMVECTOR desiredCameraPosition = XMVector3TransformCoord(cameraOffset, rotationMatrix);
    desiredCameraPosition = XMVectorAdd(desiredCameraPosition, characterPosition);

    // Clamp the cameraLag value between 0 and 1
    float clampedCameraLag = std::clamp(cameraLag, 0.0f, 1.0f);

    // Smooth camera movement
    XMVECTOR smoothedPosition = XMVectorLerp(camPosition, desiredCameraPosition, clampedCameraLag * delta);

    // Update the camera position
    camPosition= smoothedPosition;

    // Calculate the camera target (where the camera is looking) //changed to z
    XMVECTOR cameraTarget = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
    cameraTarget = XMVectorAdd(cameraTarget, characterPosition);

    // Update the camera target
    camTarget = cameraTarget;

    // Calculate the view matrix
    XMVECTOR cameraUpDirection = XMVector3TransformNormal(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationMatrix);

    // Update the camera's up vector
    camUp = cameraUpDirection;

    // Update the view matrix
    camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
   
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

void PerspectiveCamera::AddYaw(float angle)
{
    camYaw += angle;
    UpdateVectors();
}

void PerspectiveCamera::AddPitch(float angle)
{
    camPitch += angle;

    camPitch = std::clamp(camPitch, -XM_PIDIV2 + 0.1f, XM_PIDIV2 - 0.1f); // Add a small offset to avoid precision issues
    UpdateVectors();
}

void PerspectiveCamera::UpdateVectors()
{
    // Calculate the new camera orientation from the yaw and pitch angles
    XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR cameraDirection = XMVectorSet(cos(camPitch) * sin(camYaw), sin(camPitch), cos(camPitch) * cos(camYaw), 0.0f);
    XMVECTOR cameraRight = XMVector3Cross(upDirection, cameraDirection);

    // Update the camera's orientation vectors
    camUp = upDirection;
    camForward = cameraDirection;
    camRight = cameraRight;
}
