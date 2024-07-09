#pragma once
#include "stdafx.h"
#include "Graphics\ErrorEx.h"
#include "Graphics\Camera\CameraController.h"
#include "window\Window.h"
#include "Sample\Cube.h"
#include <Graphics/Camera/FreeLook.h>
#include "Graphics/Camera/ThirdPerson.h"
#include "Graphics/Camera/FirstPerson.h"
#include "Scene/Player.h"

class Input
{
public:
	Input(const std::tuple<HWND,HINSTANCE>& win);
	~Input();
	void DetectInputFree(float time, FreeLook* cam);
	void DetectInputThird(float time, ThirdPerson* cam, Player& player);
	void DetectInputFirst(float time, FirstPerson* cam);
private:
	HWND m_hwnd;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> pKeyboard;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> pMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8  DirectInput;
	std::unique_ptr<CameraController> controller;

};

