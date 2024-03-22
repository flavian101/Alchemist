#pragma once
#include "stdafx.h"
#include "Graphics/ErrorEx.h"
#include "Graphics/PerspectiveCamera.h"
#include "window\Window.h"

class Input
{
public:
	Input(Window& win);

	void DetectInput(float time, PerspectiveCamera& cam);
private:
	HWND m_hwnd;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> pKeyboard;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> pMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8  DirectInput;

};

