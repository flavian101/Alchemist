#pragma once
#include "Window.h"
#include "models/Triangle.h"
#include "ImguiManager.h"
#include "Graphics/FrameTime.h"
#include "Graphics/Camera.h"
#include "Input\Input.h"
#include <vector>


class App
{
public:
	App(HINSTANCE hInstance, int showWnd);

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
	Triangle triangle;
	bool showDemoWindow = true;
	Camera camera;
	std::unique_ptr<Input> input;
};