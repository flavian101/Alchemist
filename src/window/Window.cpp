#include "Window.h"
#include "imgui/imgui_impl_win32.h"
#include "Graphics/Graphics.h"
#include "Graphics/Renderer.h"

Window::Window(LPCWSTR windowTitle, LPCWSTR windowClass, int Width, int Height)
	:
	m_hwnd(nullptr),
	m_hInstance(GetModuleHandle(nullptr)),
	m_windowTitle(windowTitle),
	m_windowClass(windowClass),
	m_width(Width),
	m_height(Height)
{
	if (!Initialize())
	{
		MessageBox(m_hwnd, L"failed to create Window", L"ERROR", MB_OK | MB_ICONERROR);
	}
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(m_hwnd);

}
Graphics& Window::GetInstance()
{
	//static Graphics graphics(hwnd);
	if (!pGfx)
	{
		MessageBoxA(m_hwnd, "error", "No Graphics Application", MB_ICONEXCLAMATION| MB_OK);
	}
	return *pGfx;
}

bool Window::Initialize()
{
	typedef struct _WNDCLASS {
		UINT cbSize;
		UINT style;
		WNDPROC lpfnWndProc;
		int cbClsExtra;
		int cbWndExtra;
		HANDLE hInstance;
		HICON hIcon;
		HCURSOR hCursor;
		HBRUSH hbrBackground;
		LPCTSTR lpszMenuName;
		LPCTSTR lpszClassName;
	} WNDCLASS;

	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = wc.hIconSm = static_cast<HICON>(LoadImage(
		m_hInstance, MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 256, 201, 0
	));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_windowClass;
	wc.hIconSm = nullptr;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	RECT workArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

	// Set width and height based on work area
	m_width = workArea.right - workArea.left;
	m_height = workArea.bottom - workArea.top;

	// Ensure we do not expand beyond work area (no AdjustWindowRectEx needed for WS_POPUP)
	LONG newWidth = m_width;
	LONG newHeight = m_height;

	// Create window (borderless)
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,  // Ensure window appears on the taskbar
		m_windowClass,
		m_windowTitle,
		WS_POPUP | WS_VISIBLE,  // WS_POPUP for borderless mode
		workArea.left, workArea.top,  // Position at top-left of work area
		newWidth, newHeight,
		NULL, NULL, m_hInstance, this
	);

	if (!m_hwnd)
	{
		MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	// Show and update window
	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hwnd);
	SetFocus(m_hwnd);
	ImGui_ImplWin32_Init(m_hwnd);

	pGfx = std::make_unique<Graphics>(m_hwnd,newWidth,newHeight);
	pGfx->SetWin(std::make_tuple(m_hwnd, m_hInstance));

	
    return true;
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(1);
	return {};
}

HWND Window::GetHwnd() const
{
	
	return m_hwnd;
}

HINSTANCE Window::GetHinstance() const
{
	return m_hInstance;
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//create parameter passed in form CreateWindow() to store window class pointer at WinApi side
	if (msg == WM_NCCREATE)
	{
		//extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//set WinAPI-manged user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		//forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);

}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		m_width = LOWORD(lParam);
		m_height = HIWORD(lParam);
		//::GetInstance().SetWin(std::make_tuple(hWnd, m_hInstance));
	    //Graphics::GetInstance().GetDeviceResources()->Resize(m_width, m_height);
		
			//Gfx().GetDeviceResources()->Resize(m_width, m_height);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{}
		break;		
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}