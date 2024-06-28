#pragma once
#include <string>
#include <map>
#include <vector>
#include <imgui.h>
#include "Graphics/Camera/FreeLook.h"
#include "Graphics/Camera/FirstPerson.h"
#include "Graphics/Camera/ThirdPerson.h"
#include <Graphics/Camera/OrthographicCamera.h>
#include <Graphics/Graphics.h>
#include "Input\Input.h"


class SceneCamera {
public:
    SceneCamera(const std::string& name, Graphics& g);
    ~SceneCamera();

    void SetPerspectiveCamera(std::unique_ptr<PerspectiveCamera> newPerspectiveCamera);
    void SetOrthographicCamera(std::unique_ptr < OrthographicCamera> newOrthographicCamera);
    void Update(float delta,Player& player);
    void Render();
    Camera* getActiveCamera() const;
    SceneCamera* GetSelectedCamera();
    bool isPerspectiveCamera() const;
    PerspectiveCamera* GetPerspective();
    OrthographicCamera* GetOrthographic();
    void CreateNewCamera(const std::string& name, bool perspective);
    const std::map<std::string, SceneCamera*>& GetCameras() const;
    void ControlWindow();

private:
    std::string m_name;
    Graphics& m_graphics;
    std::unique_ptr<PerspectiveCamera > m_perspectiveCamera;
    std::unique_ptr<OrthographicCamera> m_orthographicCamera;
    SceneCamera* m_selectedCamera;
    std::unique_ptr<Input> input;
    bool isPerspective;
    bool showCreateWindow = false;
    static std::map<std::string, SceneCamera*> m_cameras;
    static std::vector<std::string> m_cameraNames;
    friend class Scene;
    friend class SceneSerializer;
};
