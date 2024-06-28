#pragma once
#include "PerspectiveCamera.h"

class FreeLook : public PerspectiveCamera
{
public:
	FreeLook();
	void SetFreeLook(float FOV,float aspectRatio, float nearPlane, float farPlane);

	void Update(float deltaTime) override;
	void ControlWindow() override;
	void Move(float leftRight, float backForward, float upDown, float speed, float time);
	void RotateYaw(float amount);
	void RotatePitch(float amount);
	//accessors
	float GetMoveLeftRight() const { return moveLeftRight; }
	float GetMoveBackForward() const { return moveBackForward; }
	float GetMoveUpDown() const { return moveUpDown; }
	void SetMoveLeftRight(float value) { moveLeftRight = value; }
	void SetMoveBackForward(float value) { moveBackForward = value; }
	void SetMoveUpDown(float value) { moveUpDown = value; }
	float GetCameraSpeed() { return cameraSpeed; }


private:
	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
	float moveUpDown = 0.0f;
	float cameraSpeed;

};

