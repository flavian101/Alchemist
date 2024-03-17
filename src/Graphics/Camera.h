#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
#include "window\ImguiManager.h"


using namespace DirectX;

enum class CameraMode
{
	FreeLook,
	ThirdPerson
};


class Camera
{
	
public:
	Camera(float aspectRatio = 9.0f / 16.0f, float nearPlane = 1.0f, float farPlane = 1000.0f,
		const DirectX::XMVECTOR& position = DirectX::XMVectorSet(0.0f, 5.0f, -10.0f, 1.0f),
		const DirectX::XMVECTOR& target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		const DirectX::XMVECTOR& up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));


	
	void SetCameraMode(CameraMode mode);

	DirectX::XMMATRIX GetView()const;
	DirectX::XMVECTOR GetPos() const;
	DirectX::XMVECTOR GetTarget() const;
	virtual DirectX::XMMATRIX GetProjectionMatrix() const = 0;
	void SetPosition(const DirectX::XMVECTOR& position);
	void SetTarget(const DirectX::XMVECTOR& target);

	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
	float moveUpDown = 0.0f;

	float camYaw = 0.0f;
	float camPitch = 0.0f;
	float camRoll = 0.0f;

protected:
	XMMATRIX camView;
	XMMATRIX camProjection;
	XMMATRIX camRotationMatrix;
	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;
	CameraMode cameraMode;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;
	


};

