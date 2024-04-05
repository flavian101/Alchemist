#include "Input.h"
#include <algorithm>


Input::Input(Window& win)
	:
	m_hwnd(win.GetHwnd()),
    controller(nullptr)
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

    controller = new CameraController();

}

void Input::DetectInput(float time, PerspectiveCamera& cam, Cube& character)
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

    // Update camera position and target based on the current camera mode
    if (cam.GetCameraMode() == PerspectiveCamera::FreeLook)
    {
        // Define movement speed
        float speed = cam.GetCameraSpeed();

        // Check if the middle mouse button is pressed
        if (mouseCurrState.rgbButtons[2] & 0x80)
        {
            // Camera movement
            if (keyboardState[DIK_A] & 0x80)
            {
                cam.SetMoveLeftRight(cam.GetMoveLeftRight() - speed);
            }
            else if (keyboardState[DIK_D] & 0x80)
            {
                cam.SetMoveLeftRight(cam.GetMoveLeftRight() + speed);
            }
            if (keyboardState[DIK_W] & 0x80)
            {
                cam.SetMoveBackForward(cam.GetMoveBackForward() + speed);
            }
            else if (keyboardState[DIK_S] & 0x80)
            {
                cam.SetMoveBackForward(cam.GetMoveBackForward() - speed);
            }
            
            if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
            {
                // Camera rotation
                cam.RotateYaw(mouseCurrState.lX * 0.001f);
                cam.RotatePitch(mouseCurrState.lY * -0.001f);
            }
             if (keyboardState[DIK_E] & 0x80)
            {
                controller->CameraShake(&cam, 0.02f, 10.0f, time); // Adjust intensity and duration as needed
            }
        }
        else
        {
            // Reset movement if the middle mouse button is not pressed
            cam.SetMoveLeftRight(0.0f);
            cam.SetMoveBackForward(0.0f);
        }
        cam.FreeLookCamera(time);
    }
    else if (cam.GetCameraMode() == PerspectiveCamera::ThirdPerson)
    {
        // Define movement speed and rotation sensitivity (adjust these as needed)
        float moveSpeed = cam.GetCameraSpeed();
        float rotationSpeed = 45.0f;
        XMVECTOR characterPosition = character.GetTranslation();
        XMVECTOR characterOrientation = character.GetRotation();

        // Convert the orientation quaternion to a forward direction vector
        XMVECTOR forwardDirection = XMVector3Rotate(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), characterOrientation);
        // Camera movement based on keyboard input
        if (keyboardState[DIK_W] & 0x80)
        {
            character.Move(forwardDirection, moveSpeed, time);
        }
        else if (keyboardState[DIK_S] & 0x80)
        {
            character.Move(-forwardDirection, moveSpeed, time);
        }
        if (keyboardState[DIK_A] & 0x80)
        {
            character.Rotate(-rotationSpeed * time);
        }
        else if (keyboardState[DIK_D] & 0x80)
        {
            character.Rotate(rotationSpeed * time);
        }

        if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
        {
            float yawAngle = mouseCurrState.lX * rotationSpeed * time;
            float pitchAngle = mouseCurrState.lY * -rotationSpeed * time;
            // Limit pitch to prevent flipping over
            pitchAngle = std::clamp(pitchAngle, -XM_PIDIV2, XM_PIDIV2);
            // Update camera yaw and pitch
            cam.AddYaw(yawAngle);
            cam.AddPitch(pitchAngle);


            mouseLastState = mouseCurrState;
        }
        cam.UpdateCharacterPosition(characterPosition, characterOrientation);
        cam.UpdateThirdPersonCamera(time);
    }

}
