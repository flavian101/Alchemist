#pragma once
#include "MathUtils\MathUtils.h"
#include "stdafx.h"
#include "models/Transform.h"
class Graphics;


class TransformComponent
{
public:
	TransformComponent(Graphics& g);
	~TransformComponent();
	void Update(float deltaTime);
	void Bind();
	void ControlWindow();
	XMMATRIX GetTransform()const;
	XMVECTOR GetTranslation()const;
	XMVECTOR GetRotation()const;
	XMVECTOR GetScale()const;
	void setTranslation(const XMFLOAT3& translation);
	void setRotation(const XMFLOAT4& rotation);
	void setScale(const XMFLOAT3& scale);


private:
	Transform m_transform;
};

