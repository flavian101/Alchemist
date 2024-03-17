#pragma once
#include "Camera.h"

class PerspectiveCamera :public Camera
{
public:
	PerspectiveCamera(float FOV);
	void SetCamera(float FOV,float aspectRatio, float nearPlane, float farPlane);
	DirectX::XMMATRIX GetProjectionMatrix() const override;

	void FreeLook();
	void ThirdPersonCamera(const DirectX::XMVECTOR& targetPosition, float distance, float height);

	void ContrlWindow();

private:
	float m_FOV;
};

