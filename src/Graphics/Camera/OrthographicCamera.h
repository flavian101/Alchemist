#pragma once
#include "Camera.h"

class OrthographicCamera : public Camera
{
public:
	OrthographicCamera(float width = 800.0f, float height = 600.0f,
		const DirectX::XMVECTOR& position = DirectX::XMVectorSet(0.0f, 5.0f, 10.0f, 1.0f),
		const DirectX::XMVECTOR& target = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
		const DirectX::XMVECTOR& up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	void SetCamera(float width, float height, float nearZ, float farZ);
	DirectX::XMMATRIX GetProjectionMatrix() const override;

	void Update(float delta)override;
	DirectX::XMMATRIX GetView()const override;
	void ControlWindow()override;
	DirectX::XMVECTOR GetPos() const;
	DirectX::XMVECTOR GetTarget() const;


private:
	float m_width;
	float m_height;
	

};

