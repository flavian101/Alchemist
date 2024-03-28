#pragma once
#include "stdafx.h"
#include "Graphics\ErrorEx.h"
#include "Graphics\Camera\CameraController.h"
#include "window\Window.h"
#include "Sample\Cube.h"

class Input
{
public:
	Input(Window& win);

	void DetectInput(float time, PerspectiveCamera& cam, Cube& character);
private:
	HWND m_hwnd;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> pKeyboard;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> pMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8  DirectInput;
	CameraController* controller;

};

