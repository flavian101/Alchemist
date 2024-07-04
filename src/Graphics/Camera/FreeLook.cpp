#include "FreeLook.h"

FreeLook::FreeLook()
{
	mode = freeLook;
	this->cameraSpeed = 10.0f;

}

void FreeLook::SetFreeLook(float FOV,float aspectRatio, float nearPlane, float farPlane)
{
	m_FOV = FOV;
	m_aspectRatio = aspectRatio;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
}

void FreeLook::Update(float deltaTime)
{
	mode = freeLook;
	PerspectiveCamera::Update(deltaTime);
	Move(moveLeftRight, moveBackForward, moveUpDown, cameraSpeed, deltaTime);
	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;
	moveUpDown = 0.0f;
}

void FreeLook::ControlWindow()
{
	PerspectiveCamera::ControlWindow();
	ImGui::Text("FreeLook Settings");
	Math::ImGuiDragXMVector3("position", camPosition);
	ImGui::PushItemWidth(100);
	ImGui::InputFloat("speed", &cameraSpeed);
	ImGui::PopItemWidth();
}

void FreeLook::Move(float leftRight, float backForward, float upDown, float speed, float time)
{
	DirectX::XMVECTOR moveVec = leftRight * camRight + backForward * camForward + upDown * camUp;
	moveVec *= speed * time;
	camPosition += moveVec;
	camTarget = camPosition + camForward;
	camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

void FreeLook::RotateYaw(float amount)
{
	camYaw += amount;

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

void FreeLook::RotatePitch(float amount)
{
	camPitch += amount;

	float minPitch = -DirectX::XM_PIDIV2 + 0.1f;
	float maxPitch = DirectX::XM_PIDIV2 - 0.1f;

	// Clamp camPitch within the specified range
	camPitch = std::clamp(camPitch, minPitch, maxPitch);

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


