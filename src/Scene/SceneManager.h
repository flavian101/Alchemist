#pragma once
#include "Graphics\Graphics.h"
#include <unordered_map>
#include "Scene\Scene.h"
#include "dataPack/SceneSerializer.h"


class SceneManager
{
public:
	SceneManager(Graphics &g,Window& win);
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
    void Update(float deltaTime);

    // Render the active scene
    void Render();

    void ControlWindow();


private:
	Graphics& m_graphics;
    Window& m_window;
    std::vector<Scene*> scenes;
    Scene* activeScene;
    Utils::Texture* thumbnail;
    bool showSceneWindow = true;
    bool createScenePopup = false;
    SceneSerializer* serializer;



};

