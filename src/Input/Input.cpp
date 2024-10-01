#include "Input.h"
#include <algorithm>


Input::Input(const std::tuple<HWND, HINSTANCE>& win)
	:
	m_hwnd(std::move(std::get<0>(win))),
    controller(nullptr)
{
	CHECK_RESULT(DirectInput8Create(std::get<1>(win), DIRECTINPUT_VERSION,
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

    controller = std::make_unique<CameraController>();

}

Input::~Input()
{
    pKeyboard.Reset();
    pMouse.Reset();

}

void Input::DetectInputFree(float time, FreeLook* cam)
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

	// Define movement speed
	float speed = cam->GetCameraSpeed();

    // Check if the middle mouse button is pressed
    if (mouseCurrState.rgbButtons[2] & 0x80)
    {
        // Camera movement
        if (keyboardState[DIK_A] & 0x80)
        {
            cam->SetMoveLeftRight(cam->GetMoveLeftRight() - speed);
        }
        else if (keyboardState[DIK_D] & 0x80)
        {
            cam->SetMoveLeftRight(cam->GetMoveLeftRight() + speed);
        }
        if (keyboardState[DIK_W] & 0x80)
        {
            cam->SetMoveBackForward(cam->GetMoveBackForward() + speed);
        }
        else if (keyboardState[DIK_S] & 0x80)
        {
            cam->SetMoveBackForward(cam->GetMoveBackForward() - speed);
        }

        if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
        {
            // Camera rotation
            cam->RotateYaw(mouseCurrState.lX * 0.001f);
            cam->RotatePitch(mouseCurrState.lY * -0.001f);
        }
        if (keyboardState[DIK_E] & 0x80)
        {
            controller->CameraShake((PerspectiveCamera*)cam, 0.02f, 10.0f, time); // Adjust intensity and duration as needed
        }
    }
    else
    {
        // Reset movement if the middle mouse button is not pressed
        cam->SetMoveLeftRight(0.0f);
        cam->SetMoveBackForward(0.0f);
    }
    cam->Update(time);
  

}

void Input::DetectInputThird(float time, ThirdPerson* cam, Player& player)
{
    DIMOUSESTATE mouseCurrState;

    BYTE keyboardState[256];

    pKeyboard->Acquire();
    pMouse->Acquire();

    pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
    pKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

   //
   // XMVECTOR characterPosition = player.GetTranslation();
   // XMVECTOR characterOrientation = player.GetRotation();
   //
   // // Convert the orientation quaternion to a forward direction vector
   // XMVECTOR forwardDirection = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), characterOrientation);
   //
   //
   // // Camera movement based on keyboard input
   // if (keyboardState[DIK_W] & 0x80) {
   //     player.Move(forwardDirection, cam->GetMoveSpeed(), time);
   // }
   // else if (keyboardState[DIK_S] & 0x80) {
   //     player.Move(-forwardDirection, cam->GetMoveSpeed(), time);
   // }
   // if (keyboardState[DIK_A] & 0x80) {
   //     player.Rotate(-cam->GetRotationSpeed()*time, 0.0f);
   // }
   // else if (keyboardState[DIK_D] & 0x80) {
   //     player.Rotate(cam->GetRotationSpeed() * time, 0.0f);
   // }

    if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY)) {
        float yawAngle = mouseCurrState.lX * cam->GetRotationSpeed() * time;
        float pitchAngle = mouseCurrState.lY * cam->GetRotationSpeed() * time;

        // Update camera yaw and pitch
        cam->AddYaw(yawAngle);
        cam->AddPitch(pitchAngle);

        mouseLastState = mouseCurrState;
    }
  //  cam->SetPlayerPosition(player.GetTranslation(), player.GetRotation());
    cam->Update(time);

}

void Input::DetectInputFirst(float time, FirstPerson* cam)
{
    DIMOUSESTATE mouseCurrState;
    BYTE keyboardState[256];

    pKeyboard->Acquire();
    pMouse->Acquire();

    pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
    pKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

    cam->ProcessKeyboardInput(keyboardState, time);

    if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY)) {
        float deltaX = static_cast<float>(mouseCurrState.lX);
        float deltaY = static_cast<float>(mouseCurrState.lY);
        cam->ProcessMouseInput(deltaX, deltaY);
        mouseLastState = mouseCurrState;
    }

    cam->Update(time);
}


