#pragma once
#include "Window.h"
#include "ImguiManager.h"
#include "Graphics/FrameTime.h"
#include <vector>
#include "Scene\SceneManager.h"
#include "Sample\SampleScene.h"

class App
{
public:
	App(HINSTANCE hInstance, int showWnd);
	~App();
	int createLoop();

private:
	void Render();
	void showImguiDemoWindow();
	//
private:
	FrameTime timer;
	float speedfactor = 1.0f;
	ImguiManager imgui;
	Window window;
	bool showDemoWindow = true;
	//SceneManager manager;
	SampleScene s_scene;
	
};