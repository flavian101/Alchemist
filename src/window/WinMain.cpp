#include "stdafx.h"
#include "App.h"
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{

	return App(hInstance, nCmdShow).createLoop();
}