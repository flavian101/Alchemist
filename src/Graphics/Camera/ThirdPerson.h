#pragma once
#include "PerspectiveCamera.h"

class ThirdPerson : public PerspectiveCamera
{
public:
	ThirdPerson();

	void SetThirdPerson(float FOV, float aspectRatio, float nearPlane, float farPlane);
	void Update(float deltaTime)override;
	void ControlWindow()override;
	void SetPlayerPosition(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& orientation);
	void AddYaw(float amount);
	void AddPitch(float amount);
	//accessors
	XMVECTOR GetForwardDirection()const;
	XMVECTOR GetRightDirection()const;
	void setYaw(float yaw) { camYaw = yaw; }
	float GetYaw() { return camYaw; }
	void SetPitch(float pitch) { camPitch = pitch; }
	float GetPitch() { return camPitch; }
	float GetCamRoll() { return camRoll; }
	void SetRoll(float roll) { camRoll = roll; }
	float GetMoveSpeed() {return moveSpeed;}
	float GetRotationSpeed() { return rotationSpeed; }

private:
	bool FollowPlayer;
	float distance;
	float height;
	float cameraLag;
	float moveSpeed;
	float rotationSpeed;
	DirectX::XMVECTOR playerPosition;
	DirectX::XMVECTOR playerOrientation;
};

