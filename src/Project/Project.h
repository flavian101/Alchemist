#pragma once
#include "Scene/SceneManager.h"
#include <string>
#include <vector>
#include <memory>
#include "window/Window.h"

class Project {
public:
    Project(Window& win,const std::string& name, const std::string& rootDir);
    ~Project();
    const std::string& GetName() const;
    const std::string& GetRootDirectory() const;
    SceneManager* GetSceneManager();

    void Update(Graphics& gfx,float deltaTime);
	void Render(Graphics& gfx);

    void Load();  
    void Save();  

private:
    std::string m_name;
    std::string m_rootDirectory;
    SceneManager* manager;
};