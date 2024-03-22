#pragma once
#include "Graphics\PerspectiveCamera.h"
#include "Graphics\OrthographicCamera.h"

class SceneCamera
{
public:
	SceneCamera(PerspectiveCamera* perspectiveCamera); 

	SceneCamera(OrthographicCamera* orthographicCamera);

	void SetPerspectiveCamera(PerspectiveCamera* newPerspectiveCamera);
	void SetOrthographicCamera(OrthographicCamera* newOrthographicCamera);

	Camera* getActiveCamera() const;

	bool isPerspectiveCamera();

	PerspectiveCamera* GetPerspective();
	OrthographicCamera* GetOthorgraphic();
	void ControlWindow();

private:
	PerspectiveCamera* m_perspectiveCamera;
	OrthographicCamera* m_orthographicCamera;
	bool isPerspective;
};

