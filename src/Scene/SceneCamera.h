#pragma once
#include <string>
#include <map>
#include <vector>
#include <imgui.h>
#include <Graphics/Camera/PerspectiveCamera.h>
#include <Graphics/Camera/OrthographicCamera.h>
#include <Graphics/Graphics.h>

class SceneCamera {
public:
    SceneCamera(const std::string& name, Graphics& g, bool perspective = true);
    ~SceneCamera();

    void SetPerspectiveCamera(PerspectiveCamera* newPerspectiveCamera);
    void SetOrthographicCamera(OrthographicCamera* newOrthographicCamera);
    void Update(float delta);
    void Render();
    Camera* getActiveCamera() const;
    SceneCamera* GetSelectedCamera() const;
    bool isPerspectiveCamera() const;
    PerspectiveCamera* GetPerspective();
    OrthographicCamera* GetOrthographic();
    void CreateNewCamera(const std::string& name, bool perspective);
    const std::map<std::string, SceneCamera*>& GetCameras() const;
    void ControlWindow();

private:
    std::string m_name;
    Graphics& m_graphics;
    PerspectiveCamera* m_perspectiveCamera;
    OrthographicCamera* m_orthographicCamera;
    SceneCamera* m_selectedCamera;
    bool isPerspective;
    bool showCreateWindow = false;
    static std::map<std::string, SceneCamera*> m_cameras;
    static std::vector<std::string> m_cameraNames;
    friend class Scene;
    friend class SceneSerializer;
};
