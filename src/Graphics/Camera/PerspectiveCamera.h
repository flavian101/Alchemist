#pragma once

#include "Camera.h"
#include <algorithm>


class PerspectiveCamera :public Camera
{
public:
	enum CameraMode {
		FreeLook,
		ThirdPerson
	};
	PerspectiveCamera(float aspectRatio = (9.0f / 16.0f), float nearPlane = 1.0f, float farPlane = 1000.0f,
		const DirectX::XMVECTOR& position = DirectX::XMVectorSet(0.0f, 5.0f, -10.0f, 1.0f),
		const DirectX::XMVECTOR& target =   DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		const DirectX::XMVECTOR& up =       DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));


	void SetCamera(float FOV,float aspectRatio, float nearPlane, float farPlane);
	DirectX::XMMATRIX GetProjectionMatrix() const override;
	void Update(float delta) override;
	DirectX::XMMATRIX GetView()const override;
	void FreeLookCamera(float delta);
	void FirstPersonCamera(float delta);
	void ThirdPersonCamera(float delta);
	void Reset();


	void SetView(DirectX::XMMATRIX view);


	DirectX::XMVECTOR GetPos() const;
	DirectX::XMVECTOR GetTarget() const;
	DirectX::XMVECTOR GetUp() const;
	void SetPosition(DirectX::XMVECTOR& position);
	void SetTarget(DirectX::XMVECTOR& target);
	void SetUP(DirectX::XMVECTOR& up);
	// Move the camera in the specified direction
	void Move(float leftRight, float backForward, float upDown, float speed, float time);

	// Rotate the camera horizontally (yaw)
	void RotateYaw(float amount);

	// Rotate the camera vertically (pitch)
	void RotatePitch(float amount);
	void setYaw(float yaw) { camYaw = yaw; }
	float GetYaw() { return camYaw; }
	void SetPitch(float pitch);
	float GetPitch() { return camPitch; }
	float GetCamRoll() { return camRoll; }

	// Getter methods for movement variables
	float GetMoveLeftRight() const { return moveLeftRight; }
	float GetMoveBackForward() const { return moveBackForward; }
	float GetMoveUpDown() const { return moveUpDown; }

	// Setter methods for movement variables
	void SetMoveLeftRight(float value) { moveLeftRight = value; }
	void SetMoveBackForward(float value) { moveBackForward = value; }
	void SetMoveUpDown(float value) { moveUpDown = value; }

	
	void ControlWindow() override;
	void ThirdPersonWindow();
	CameraMode GetCameraMode() const { return mode; }
	
	float GetFOV() const;
	void setFOV(float fov);

public:
	CameraMode mode = FreeLook;
	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	
	XMMATRIX camRotationMatrix;
	

private:
	float m_FOV;

	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
	float moveUpDown = 0.0f;
	float camYaw = 0.0f;
	float camPitch = 0.0f;
	float camRoll = 0.0f;


	float cameraSpeed = 2.0f;
	float distance = 15.0f;

	


	
};

