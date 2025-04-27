#pragma once
#include "Window.h"
#include "ImguiManager.h"
#include "Graphics/FrameTime.h"
#include <vector>
#include "Graphics/Graphics.h"
#include "LoginWindow.h"
#include "Project/ClientProjectManager.h"
#include <memory>
#include "network/Client.h"

class ClientApp
{
public:
	ClientApp();
	~ClientApp();
	int createLoop();


private:
	void Render();
	void GameProjects();
	//
private:
	ImguiManager imgui;
	Window window;
	LoginWindow loginWin;
	Client client;
	ClientProjectManager projectManager;
	float speedFactor = 1.0f;
	bool showDemoWindow = true;
	bool loggedIn;

	
};

