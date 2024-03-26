// SceneCamera.h

#pragma once

#include <vector>
#include <string>
#include "Graphics/Camera/PerspectiveCamera.h"
#include "Graphics/Camera/OrthographicCamera.h"
#include <map>



class SceneCamera {
public:
    SceneCamera(const std::string& name, PerspectiveCamera* perspectiveCamera);
    SceneCamera(const std::string& name, OrthographicCamera* orthographicCamera);
    ~SceneCamera();

    void SetPerspectiveCamera(PerspectiveCamera* newPerspectiveCamera);
    void SetOrthographicCamera(OrthographicCamera* newOrthographicCamera);
    Camera* getActiveCamera() const;
    SceneCamera* GetSelectedCamera() const;
    bool isPerspectiveCamera();
    PerspectiveCamera* GetPerspective();
    OrthographicCamera* GetOrthographic();
    void CreateNewCamera(const std::string& name, bool perspective);
    static const std::map<std::string, SceneCamera*>& GetCameras();

    void ControlWindow();

private:
    friend PerspectiveCamera;
    friend OrthographicCamera;
    PerspectiveCamera* m_perspectiveCamera;
    OrthographicCamera* m_orthographicCamera;
    bool isPerspective;
    std::string m_name;
    std::vector<SceneCamera*> m_cameraList; // Vector to store camera pointers
    std::vector<std::string> m_cameraNames; // Vector to store camera names
    static std::map<std::string, SceneCamera*> m_cameras;
    SceneCamera* m_selectedCamera;

};
