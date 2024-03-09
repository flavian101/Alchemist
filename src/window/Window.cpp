#include "Window.h"
#include "imgui_impl_win32.h"


Window::Window(HINSTANCE hInstance, int nCmdShow, LPCWSTR windowTitle, LPCWSTR windowClass, int Width, int Height)
	:
	m_hInstance(hInstance),
	m_nShowWnd(nCmdShow),
	m_windowTitle(windowTitle),
	m_windowClass(windowClass),
	m_width(Width),
	m_height(Height),
	m_hwnd(nullptr)
{
	if (!Initialize())
	{
		MessageBox(m_hwnd, L"failed to create Window", L"ERROR", MB_OK | MB_ICONERROR);
	}
	
}

Window::~Window()
{
	if (m_hwnd)
	{
		ImGui_ImplWin32_Shutdown();

		DestroyWindow(m_hwnd);
	}
}
Graphics& Window::Gfx()
{
	if (!pGfx)
	{
		MessageBox(m_hwnd, L"failed initialize graphics Object", L"error", MB_OK);
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
	wc.hIcon = nullptr;
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

	//create windows rect
	RECT initialRect = { 0,0,m_width, m_height };
	AdjustWindowRectEx(&initialRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);
	LONG initialWidth = initialRect.right - initialRect.left;
	LONG initialHeight = initialRect.bottom - initialRect.top;
	m_hwnd = CreateWindowEx(
		NULL,
		m_windowClass,
		m_windowTitle,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		initialWidth, initialHeight,
		NULL,
		NULL,
		m_hInstance,
		this
	);

	if (!m_hwnd)
	{
		MessageBox(NULL, L"Error creating window",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(m_hwnd, m_nShowWnd);
	//UpdateWindow(m_hwnd);
	//SetFocus(m_hwnd);
	ImGui_ImplWin32_Init(m_hwnd);
	pGfx = std::make_unique<Graphics>(m_hwnd,m_width, m_height,false);
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
	case WM_SIZE:
		m_width = LOWORD(lParam);
		m_height = HIWORD(lParam);
		//if (pGfx) // Check if pGfx is initialized
		//{
		//	pGfx->Resize(m_width, m_height);
		//}
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}