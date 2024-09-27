#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>


class Graphics;
class Camera;
class PerspectiveCamera;
class FreeLook;
class FirstPersin;
class ThirdPerson;
class OrthographicCamera;
class Input;
class Player;

class SceneCamera {
public:
    SceneCamera(const std::string& name, Graphics& g);
    ~SceneCamera();

    void SetPerspectiveCamera(std::unique_ptr<PerspectiveCamera> newPerspectiveCamera);
    void SetOrthographicCamera(std::unique_ptr < OrthographicCamera> newOrthographicCamera);
    void Update(float delta,Player& player);
    void Render(Graphics& gfx);
    Camera* getActiveCamera() const;
    SceneCamera* GetSelectedCamera();
    bool isPerspectiveCamera() const;
    PerspectiveCamera* GetPerspective();
    OrthographicCamera* GetOrthographic();
    void CreateNewCamera(const std::string& name, bool perspective, Graphics& gfx);
    const std::map<std::string, SceneCamera*>& GetCameras() const;
    void ControlWindow(Graphics& gfx);

private:
    std::string m_name;
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
