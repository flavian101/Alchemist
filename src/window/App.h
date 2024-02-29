#pragma once
#include "Window.h"
#include "models/Triangle.h"
#include "ImguiManager.h"


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
	ImguiManager imgui;
	Window window;
	Triangle triangle;
	bool showDemoWindow = true;
};