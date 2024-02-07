#pragma once
#include "stdafx.h"
#include <optional>
#include <memory>

class Window
{
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	int m_nShowWnd;
	LPCWSTR m_windowTitle;
	LPCWSTR m_windowClass;
	int m_width;
	int m_height;

public:
	Window(HINSTANCE hInstance, int nCmdShow, LPCWSTR windowTitle,
		LPCWSTR windowClass, int Width, int Height);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();

	bool Initialize();
	static std::optional<int> ProcessMessages();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;







};