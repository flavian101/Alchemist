#pragma once
#include "stdafx.h"
#include <optional>
#include <memory>
#include <tuple>
#include "resource.h"

class Graphics;

class Window
{
	friend class Graphics;
	friend class SceneSerializer;
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	LPCWSTR m_windowTitle;
	LPCWSTR m_windowClass;
	int m_width;
	int m_height;
	//std::unique_ptr<Graphics> pGfx;


public:
	
	Window(LPCWSTR windowTitle,
		LPCWSTR windowClass, int Width, int Height);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void OnDelete();
	~Window();

	static Graphics& Gfx();
	static std::optional<int> ProcessMessages();
	HWND GetHwnd()const;
	HINSTANCE GetHinstance()const;


private:
	bool Initialize();
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;







};