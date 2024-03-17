#pragma once
#include "Graphics\PerspectiveCamera.h"
#include "Graphics\OrthographicCamera.h"

class SceneCamera
{
public:
	SceneCamera();
	PerspectiveCamera CreatePerspective(float FOV);
	OrthographicCamera CreateOthorgaphic(float width, float height);
	void ControlWindow();

private:
	float m_FOV;
	float m_width, m_height;
};

