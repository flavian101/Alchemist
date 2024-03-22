#pragma once

#include "Camera.h"

class PerspectiveCamera :public Camera
{
public:
	PerspectiveCamera(float aspectRatio = (9.0f / 16.0f), float nearPlane = 1.0f, float farPlane = 1000.0f,
		const DirectX::XMVECTOR& position = DirectX::XMVectorSet(0.0f, 5.0f, -10.0f, 1.0f),
		const DirectX::XMVECTOR& target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		const DirectX::XMVECTOR& up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));


	void SetCamera(float FOV,float aspectRatio, float nearPlane, float farPlane);
	DirectX::XMMATRIX GetProjectionMatrix() const override;
	void Update(float delta) override;
	void FreeLook();
	void ThirdPersonCamera(const DirectX::XMVECTOR& targetPosition, float distance, float height);
	void ContrlWindow();

	DirectX::XMMATRIX GetView()const override;
	DirectX::XMVECTOR GetPos() const;
	DirectX::XMVECTOR GetTarget() const;
	void SetPosition(DirectX::XMVECTOR& position);
	void SetTarget(DirectX::XMVECTOR& target);

public:
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

private:
	float m_FOV;
	


	
};

