#pragma once
#include <unordered_map>
#include "dataPack/SceneSerializer.h"
#include "dataPack/Json.hpp"
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

class ServerSceneManager
{
public:
	ServerSceneManager(Window* win);
	~ServerSceneManager();
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

	std::vector<Scene*> GetScenes() const { return scenes; }
    nlohmann::json SerializeSceneManager(const std::string& projectName, const std::string& projectDir);
    void DeserializeSceneManager(const nlohmann::json& j);

private:
    Window& m_window;;
    std::vector<Scene*> scenes;
    Scene* activeScene;
    std::unique_ptr<Utils::Texture> thumbnail;
  
    bool showSceneWindow = true;
    bool createScenePopup = false;
    SceneSerializer* serializer;

};

