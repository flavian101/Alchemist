#pragma once
#include "Graphics\camera\PerspectiveCamera.h"
#include "Graphics\camera\OrthographicCamera.h"

class SceneCamera
{
public:
	SceneCamera(PerspectiveCamera* perspectiveCamera); 

	SceneCamera(OrthographicCamera* orthographicCamera);
	~SceneCamera();
	void Add(Camera* camera);
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

