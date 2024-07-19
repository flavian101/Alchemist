#pragma once
#include <DirectXMath.h>
class ImguiManager;


using namespace DirectX;
class Camera
{
public:
	Camera() = default;
	
	virtual DirectX::XMMATRIX GetProjectionMatrix() const = 0;
	virtual DirectX::XMMATRIX GetView()const = 0;

	virtual void Update(float delta) = 0;
	virtual void ControlWindow() = 0;
protected:

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	XMMATRIX camView;
	XMMATRIX camProjection;
	XMMATRIX camRotationMatrix;

	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;
	

	

};

