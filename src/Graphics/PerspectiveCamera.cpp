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
    return DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_aspectRatio, m_nearPlane, m_farPlane);
}

void PerspectiveCamera::Update(float delta)
{
}

void PerspectiveCamera::FreeLook()
{
    using namespace DirectX;
    // Calculate the forward vector based on the camera's rotation
    XMVECTOR forwardVec = XMVectorSet(
        sinf(camYaw) * cosf(camPitch),
        sinf(camPitch),
        cosf(camYaw) * cosf(camPitch),
        0.0f
    );

    // Move the camera
    camPosition += moveLeftRight * camRight + moveBackForward * forwardVec + moveUpDown * camUp;

    // Reset movement values
    moveLeftRight = 0.0f;
    moveBackForward = 0.0f;
    moveUpDown = 0.0f;

    // Update target position
    camTarget = camPosition + forwardVec;

    // Update view matrix
    camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

void PerspectiveCamera::ThirdPersonCamera(const DirectX::XMVECTOR& targetPosition, float distance, float height)
{
    using namespace DirectX;

    XMVECTOR direction = XMVectorSubtract(camPosition, targetPosition);
    direction = XMVector3Normalize(direction);
    XMVECTOR  cameraPosition = XMVectorAdd(targetPosition, XMVectorScale(direction, -distance));
    cameraPosition = XMVectorSetY(cameraPosition, XMVectorGetY(cameraPosition));

    //update camera pos and target
    camPosition = cameraPosition;
    camTarget = targetPosition;

    //update View matrix
    camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);


}

void PerspectiveCamera::ContrlWindow()
{
    ImGui::Begin("3d Camera Type"); 

    const char* items[] = { "Free Look", "3rd Person" };
    static int currentItem = 0; // Use integer index instead of string comparison

    if (ImGui::BeginCombo("Type", items[currentItem]))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (currentItem == n); // Check against loop index directly
            if (ImGui::Selectable(items[n], is_selected))
                currentItem = n; // Update the selected item index directly
            if (is_selected)
                ImGui::SetItemDefaultFocus(); // Set initial focus when opening the combo
        }
        ImGui::EndCombo();
    }

    ImGui::End();

    // Perform actions based on the selected item after the ImGui window ends
    switch (currentItem)
    {
    case 0: // Free Look
        // Action for Free Look
        // Example: camera.SetCameraMode(CameraMode::FreeLook);
        break;
    case 1: // 3rd Person
        // Action for 3rd Person
        // Example: camera.SetCameraMode(CameraMode::ThirdPerson);
        break;
    }
}



DirectX::XMMATRIX PerspectiveCamera::GetView() const
{
    return camView;
}

DirectX::XMVECTOR PerspectiveCamera::GetPos() const
{
    return camPosition;
}

DirectX::XMVECTOR PerspectiveCamera::GetTarget() const
{
    return camTarget;
}

void PerspectiveCamera::SetPosition(DirectX::XMVECTOR& position)
{
    camPosition = position;

}

void PerspectiveCamera::SetTarget(DirectX::XMVECTOR& target)
{
    camTarget = XMVector3Normalize(target - camPosition);
}
