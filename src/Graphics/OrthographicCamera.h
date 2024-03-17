#pragma once
#include "Camera.h"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(float width, float height);
	void SetCamera(float width, float height, float nearZ, float farZ);
	DirectX::XMMATRIX GetProjectionMatrix() const override;

private:
	float m_width;
	float m_height;

};

