#pragma once
#include "Graphics.h"
#include <DirectXMath.h>

using namespace DirectX;
class Camera
{
	
public:
	Camera(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& target, const DirectX::XMVECTOR& up);

	void UpdateCamera();

	DirectX::XMMATRIX GetView()const;
	DirectX::XMVECTOR GetPos() const;
	DirectX::XMVECTOR GetTarget() const;
	void SetPosition(const DirectX::XMVECTOR& position);
	void SetTarget(const DirectX::XMVECTOR& target);

	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;

	float camYaw = 0.0f;
	float camPitch = 0.0f;

private:
	XMMATRIX camView;
	XMMATRIX camProjection;
	XMMATRIX camRotationMatrix;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;


};

