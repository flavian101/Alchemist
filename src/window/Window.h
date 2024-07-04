#pragma once
#include "stdafx.h"
#include <optional>
#include <memory>
#include "Graphics/Graphics.h"
#include <tuple>
#include "resource.h"


class Window
{
	friend class Graphics;
	friend class SceneSerializer;
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	int m_nShowWnd;
	LPCWSTR m_windowTitle;
	LPCWSTR m_windowClass;
	int m_width;
	int m_height;
	std::unique_ptr<Graphics> pGfx;


public:
	
	Window(HINSTANCE hInstance, int nCmdShow, LPCWSTR windowTitle,
		LPCWSTR windowClass, int Width, int Height);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	~Window();

	Graphics& Gfx();
	static std::optional<int> ProcessMessages();
	HWND GetHwnd()const;
	HINSTANCE GetHinstance()const;


private:
	bool Initialize();
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;







};