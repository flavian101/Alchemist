#include "Input.h"

Input::Input(Window& win)
	:
	m_hwnd(win.GetHwnd())
{
	CHECK_RESULT(DirectInput8Create(win.GetHinstance(), DIRECTINPUT_VERSION,
		IID_IDirectInput8,(void**)&DirectInput, nullptr));
	CHECK_RESULT(DirectInput->CreateDevice(GUID_SysKeyboard, pKeyboard.GetAddressOf(), nullptr));
	CHECK_RESULT(DirectInput->CreateDevice(GUID_SysMouse, pMouse.GetAddressOf(), nullptr));

	//set devices
	CHECK_RESULT(pKeyboard->SetDataFormat(&c_dfDIKeyboard));
	pKeyboard->SetCooperativeLevel((HWND)m_hwnd, DISCL_FOREGROUND );

	CHECK_RESULT( pMouse->SetDataFormat(&c_dfDIMouse));
	//CHECK_RESULT( pMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND));
	pMouse->SetCooperativeLevel((HWND)m_hwnd, DISCL_NOWINKEY | DISCL_FOREGROUND);
	//pMouse->SetCooperativeLevel(m_hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);


}

void Input::DetectInput(float time, Camera& cam)
{
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];

	pKeyboard->Acquire();
	pMouse->Acquire();

	pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
	pKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (keyboardState[DIK_ESCAPE] & 0x80)
	{
		PostMessage(m_hwnd, WM_DESTROY, 0, 0);
	}

    float speed = 0.02f * time;
    if (mouseCurrState.rgbButtons[2] & 0x80) // Middle mouse button pressed
    {
       
        if (keyboardState[DIK_A] & 0x80)
        {
            cam.moveLeftRight -= speed;
        }
        else if (keyboardState[DIK_D] & 0x80)
        {
            cam.moveLeftRight += speed;
        }
        else if (keyboardState[DIK_W] & 0x80)
        {
            cam.moveBackForward += speed;
        }
        else if (keyboardState[DIK_S] & 0x80)
        {
            cam.moveBackForward -= speed;
        }

        // Update camera rotation based on mouse movement
        if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
        {
            cam.camYaw += mouseLastState.lX * 0.001f;
            cam.camPitch += mouseCurrState.lY * -0.001f;
            mouseLastState = mouseCurrState;
        }
    }
    else
    {
        
        cam.moveLeftRight = 0.0f;
        cam.moveBackForward = 0.0f;
    }

  
		//cam.FreeLook();
	
}
