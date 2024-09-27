#pragma once
#include <unordered_map>
#include "dataPack/SceneSerializer.h"
#include "stdafx.h"
#include <string>
#include <memory>
#include <vector>

class Graphics;
class Scene;
class Window;
class SceneSerializer;
namespace Utils
{
    class Texture;
}

class SceneManager
{
public:
	SceneManager(Window* win);
	~SceneManager();
    // Add a scene to the manager
    void AddScene(Scene* scene);

    // Remove a scene from the manager
    void RemoveScene(Scene* scene);

    // Set the active scene by index
    void SetActiveScene(int index);

    // Set the active scene by name
    void SetActiveScene(const std::string& name);

    // Update the active scene
    void Update(Graphics& gfx,float deltaTime);

    // Render the active scene
    void Render(Graphics& gfx);

    void ControlWindow(Graphics& gfx);


private:
    std::vector<Scene*> scenes;
    Scene* activeScene;
    std::unique_ptr<Utils::Texture> thumbnail;
    std::unique_ptr<Utils::Texture> Logo;
    std::unique_ptr<Utils::Texture> close;
    std::unique_ptr<Utils::Texture> minimize;
    std::unique_ptr<Utils::Texture> maximize;
    bool showSceneWindow = true;
    bool createScenePopup = false;
    SceneSerializer* serializer;
    bool g_isDragging = false;
    POINT g_dragStartPoint = { 0, 0 };
    POINT g_windowStartPoint = { 0, 0 };



};

