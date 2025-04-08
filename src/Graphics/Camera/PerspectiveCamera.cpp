#include "PerspectiveCamera.h"

#include "FreeLook.h"


PerspectiveCamera::PerspectiveCamera()
    : Camera()
{
    camPosition = DirectX::XMVectorSet(0.0f, 6.0f, -16.0f, 1.0f);
    camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    m_aspectRatio = static_cast<float>(16.0f) / static_cast<float>(9.0f);
    m_nearPlane = 1.0f;
    m_farPlane = 1000.0f;
    mode = freeLook;
    m_FOV = 45.0f;
    camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
}
void PerspectiveCamera::setCamera(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
	m_FOV = FOV;
	m_aspectRatio = aspectRatio;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
}
void PerspectiveCamera::SetAspectRatio(float ratio)
{
    m_aspectRatio = ratio;
}
DirectX::XMMATRIX PerspectiveCamera::GetProjectionMatrix() const
{
    return DirectX::XMMatrixPerspectiveFovLH(XMConvertToRadians(m_FOV), m_aspectRatio, m_nearPlane, m_farPlane);
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
    const char* items[] = { "Free Look", "Third Person", "First Person" };
    int currentItem = static_cast<int>(mode);
    // Render the combo box
   // if (ImGui::BeginCombo("##CameraModeCombo", items[currentItem])) {
   //     for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
   //         bool isSelected = (currentItem == i);
   //         if (ImGui::Selectable(items[i], isSelected)) {
   //             currentItem = i;
   //         }
   //         if (isSelected)
   //             ImGui::SetItemDefaultFocus();
   //     }
   //     ImGui::EndCombo();
   //
   //     
   // }
   // mode = static_cast<CameraMode>(currentItem);
   // if (mode == thirdPerson) {
   //    // ThirdPersonWindow();
   // }
   // else if (mode == freeLook) {
   // }


    ImGui::Columns(1);
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

void PerspectiveCamera::SetPosition(const DirectX::XMVECTOR& position)
{
    camPosition = position;

}

void PerspectiveCamera::SetTarget(const DirectX::XMVECTOR& target)
{
    camTarget = target;
}

void PerspectiveCamera::SetUP(const DirectX::XMVECTOR& up)
{
    camUp = up;
}

DirectX::XMMATRIX PerspectiveCamera::GetView() const
{
    return camView;
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


