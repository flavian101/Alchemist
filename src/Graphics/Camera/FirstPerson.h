#pragma once
#include "PerspectiveCamera.h"
#include <stdafx.h>

class FirstPerson : public PerspectiveCamera
{
public:
	FirstPerson();

	void Update(float deltaTime)override;
	void ProcessKeyboardInput(const BYTE* keyboardState, float deltaTime);
	void ProcessMouseInput(float deltaX, float deltaY);
	void ControlWindow()override;
	//acessors
	void SetSpeed(float speed);
	float GetSpeed()const;
private:
	float movementSpeed;
	float mouseSensitivity;
};

