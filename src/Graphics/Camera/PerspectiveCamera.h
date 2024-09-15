#pragma once
#include "Camera.h"
#include <algorithm>
#include "MathUtils\MathUtils.h"


class FreeLook;
class ThirdPerson;

enum CameraMode {
	freeLook,
	firstPerson,
	thirdPerson
};
class PerspectiveCamera :public Camera
{
public:
	PerspectiveCamera();

	void Reset();
	void SetView(DirectX::XMMATRIX view);
	virtual void ControlWindow() override;
	//setters and getters
	void SetAspectRatio(float ratio);
	DirectX::XMMATRIX GetProjectionMatrix() const override;
	void Update(float delta) override;
	DirectX::XMMATRIX GetView()const override;
	DirectX::XMVECTOR GetPos() const;
	DirectX::XMVECTOR GetTarget() const;
	DirectX::XMVECTOR GetUp() const;
	void SetPosition(const DirectX::XMVECTOR& position);
	void SetTarget(const DirectX::XMVECTOR& target);
	void SetUP(const DirectX::XMVECTOR& up);
	CameraMode GetCameraMode() const { return mode; }
	float GetFOV() const;
	void setFOV(float fov);

	

private:
	friend class FreeLook;
	friend class ThirdPerson;
	friend class FirstPerson;
	friend class SceneSerializer;
	float m_FOV;
	float camYaw = 0.0f;
	float camPitch = 0.0f;
	float camRoll = 0.0f;
	CameraMode mode;
	XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	
};

