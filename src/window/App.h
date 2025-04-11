#pragma once
#include "Window.h"
#include "ImguiManager.h"
#include "Graphics/FrameTime.h"
#include <vector>
#include "Graphics/Graphics.h"
#include "LoginWindow.h"
#include "Project/ProjectManager.h"
#include <memory>
#include "network/Client.h"

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
	LoginWindow loginWin;
	ProjectManager projectManager;
	Client client;
	float speedFactor = 1.0f;
	bool showDemoWindow = true;
	bool loggedIn;

	
};

