#pragma once
#include "Window.h"
#include "models/Triangle.h"

class App
{
public:
	App(HINSTANCE hInstance, int showWnd);

	int createLoop();

private:
	void Render();

	//
private:
	Window window;
	Triangle triangle;
};