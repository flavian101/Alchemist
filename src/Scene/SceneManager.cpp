#include "SceneManager.h"
#include "Sample\SampleScene.h" // Include SampleScene or other scene classes here


SceneManager::SceneManager(Graphics& g)
	:
	m_graphics(g),
	activeScene(nullptr)
{
}

SceneManager::~SceneManager() {
    for (auto scene : scenes)
        delete scene;
}

void SceneManager::AddScene(Scene* scene) {
    scenes.push_back(scene);
}

void SceneManager::RemoveScene(Scene* scene) {
    auto it = std::find(scenes.begin(), scenes.end(), scene);
    if (it != scenes.end()) {
        delete* it;
        scenes.erase(it);
    }
}

void SceneManager::SetActiveScene(int index) {
    if (index >= 0 && index < scenes.size())
        activeScene = scenes[index];
}

void SceneManager::SetActiveScene(const std::string& name) {
    for (auto scene : scenes) {
        if (scene->GetName() == name) {
            activeScene = scene;
            return;
        }
    }
}

void SceneManager::Update(float deltaTime) {
    if (activeScene)
        activeScene->Update(deltaTime);
}

void SceneManager::Render() {
    if (activeScene)
        activeScene->Render();
}