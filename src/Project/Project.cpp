#include "Project.h"
#include <fstream>
#include <iostream>
#include "Scene/GameObject.h"
#include "Scene/Scene.h"

Project::Project(Window& win, const std::string& name, const std::string& rootDir)
    : m_name(name), m_rootDirectory(rootDir), 
    manager(new SceneManager(&win))
{
}

Project::~Project()
{
    delete manager;
}

const std::string& Project::GetName() const {
    return m_name;
}

const std::string& Project::GetRootDirectory() const {
    return m_rootDirectory;
}

SceneManager* Project::GetSceneManager()
{
    return manager;
}

void Project::Update(Graphics& gfx, float deltaTime)
{
	if (manager) {
		manager->Update(gfx, deltaTime);
	}
}

void Project::Render(Graphics& gfx)
{
	if (manager) {
		manager->Render(gfx);
	}
}

void Project::Load() {
    std::ifstream file(m_rootDirectory + "/"+m_name+".json");
    if (file.is_open()) {
        nlohmann::json j;
        file >> j;
        file.close();

        // Deserialize project data
        manager->DeserializeSceneManager(j);
    }
    else {
        std::cerr << "Failed to open project file for loading: " << m_rootDirectory + "/project.json" << std::endl;
    }
}

void Project::Save() {

    fs::create_directories(m_rootDirectory); // This creates all intermediate directories if needed

    std::ofstream file(m_rootDirectory + "/" + m_name + ".json");
    if (file.is_open()) {
        nlohmann::json j = manager->SerializeSceneManager(m_name,m_rootDirectory);
        file << j.dump(4);
        file.close();
    }
    else {
        std::cerr << "Failed to open project file for saving: " << m_rootDirectory + "/" + m_name + ".json" << std::endl;
    }

}

void Project::Deserialize(const std::string& jsonData) {
    try {
        nlohmann::json j = nlohmann::json::parse(jsonData);
        manager->DeserializeSceneManager(j);
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to deserialize project data: " << e.what() << std::endl;
    }
}
