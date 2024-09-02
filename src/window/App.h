#pragma once
#include "Window.h"
#include "ImguiManager.h"
#include "Graphics/FrameTime.h"
#include <vector>
#include "Scene\SceneManager.h"
#include "Graphics/Graphics.h"
class App
{
public:
	App();
	~App();
	int createLoop();


private:
	void Render();
	void GameProjects();
	//
private:
	ImguiManager imgui;
	Window window;
	FrameTime timer;
	float speedFactor = 1.0f;
	bool showDemoWindow = true;
	SceneManager sceneManager;
	
};

