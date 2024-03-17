#pragma once
#include "Graphics\Graphics.h"
#include "Scene\Scene.h"
#include <unordered_map>


class SceneManager
{
public:
	SceneManager(Graphics &g);
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
	

private:
	Graphics& m_graphics;
    std::vector<Scene*> scenes;
    Scene* activeScene;

};

