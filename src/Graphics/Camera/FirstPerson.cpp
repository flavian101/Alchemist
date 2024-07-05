#include "FirstPerson.h"

FirstPerson::FirstPerson()
	:
	movementSpeed(10.f),
	mouseSensitivity(0.001f)
{
	mode = firstPerson;
	XMVECTOR direction = XMVector3Normalize(camTarget - camPosition);
	camYaw =(float) atan2(XMVectorGetX(direction), XMVectorGetZ(direction));
	camPitch =(float) asin(XMVectorGetY(direction));
}

void FirstPerson::Update(float deltaTime)
{
	PerspectiveCamera::Update(deltaTime);
	mode = firstPerson;
	camView = XMMatrixLookAtLH(camPosition, camPosition + camForward, camUp);

}

void FirstPerson::ProcessKeyboardInput(const BYTE* keyboardState, float deltaTime)
{
	XMVECTOR moveDirection = XMVectorZero();

	if (keyboardState[DIK_W] & 0x80) {
		moveDirection += camForward;
	}
	if (keyboardState[DIK_S] & 0x80) {
		moveDirection -= camForward;
	}
	if (keyboardState[DIK_A] & 0x80) {
		moveDirection -= camRight;
	}
	if (keyboardState[DIK_D] & 0x80) {
		moveDirection += camRight;
	}
	camPosition += XMVector3Normalize(moveDirection) * movementSpeed * deltaTime;
}

void FirstPerson::ProcessMouseInput(float deltaX, float deltaY)
{
	camYaw += deltaX * mouseSensitivity;
	camPitch += deltaY * mouseSensitivity;

	camPitch = std::clamp(camPitch, -XM_PIDIV2 + 0.1f, XM_PIDIV2 - 0.1f);
	XMVECTOR front;
	front.m128_f32[0] = cos(camPitch) * sin(camYaw);
	front.m128_f32[1] = sin(camPitch);
	front.m128_f32[2] = cos(camPitch) * cos(camYaw);

	camForward = XMVector3Normalize(front);
	camRight = XMVector3Normalize(XMVector3Cross(camUp, camForward));
}

void FirstPerson::ControlWindow()
{
	PerspectiveCamera::ControlWindow();
	ImGui::Text("First Person Settings");
	ImGui::PushItemWidth(100);
	ImGui::DragFloat("moveSpeed", &movementSpeed, 1.0f, 0.0f);
	ImGui::DragFloat("mouse Senstivity", &mouseSensitivity, 0.01f, 0.001f);
	ImGui::PopItemWidth();
}

void FirstPerson::SetSpeed(float speed)
{
	movementSpeed = speed;
}

float FirstPerson::GetSpeed() const
{
	return movementSpeed;
}
