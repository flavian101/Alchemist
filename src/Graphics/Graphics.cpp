#include "Graphics.h"
#include <cassert>
#include "ErrorEx.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

Graphics::Graphics(HWND hWnd, int Width, int Height, bool FullScreen)
	:
	hWnd(hWnd),
	m_width(Width),
	m_height(Height)
{
	if (!Intitalize())
	{
		MessageBox(hWnd, L"Failed to initialize Direct3D", L"Error", MB_OK | MB_ICONEXCLAMATION);
	}
	
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());

}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();

}

void Graphics::ClearDepthColor(float red, float green, float blue)
{
	if (imguiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float color[] = {red, green, blue, 1.0f};
	pContext->ClearRenderTargetView(pRenderTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);

	//Set our Render Target
	pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDSV.Get());


}

void Graphics::Render(UINT indexCount)
{
	pContext->DrawIndexed(indexCount,0,0);
}

void Graphics::End()
{
	//imgui frame end
	if (imguiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	if (isVsyncEnabled)
	{
		CHECK_RESULT(pSwapChain->Present(1u, 0u));
	}
	else
	{
		CHECK_RESULT(pSwapChain->Present(0u, 0u));
	}
}

void Graphics::ToggleMsaa(bool enableMsaa)
{
	is4xMsaaEnabled = enableMsaa;
	Resize();
}

void Graphics::EnableImgui()
{
	imguiEnabled = true;
}

void Graphics::DisableImgui()
{
	imguiEnabled = false;
}

bool Graphics::isImguiEnabled() const noexcept
{
	return imguiEnabled;;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> Graphics::GetContext()
{
	return pContext;
}

Microsoft::WRL::ComPtr<ID3D11Device> Graphics::GetDevice()
{
	return pDevice;
}

void Graphics::controlWindow()
{
	if (ImGui::Begin("Graphics Settings"))
	{
		//ImGui::SliderFloat("Speed Factor", &speedfactor, 0.0f, 6.0f, "%.4f", 3.2f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("Vsync",& isVsyncEnabled);
		bool msaaChanged = ImGui::Checkbox("Msaa", &is4xMsaaEnabled);
		// Check if MSAA checkbox value changed
		if (msaaChanged)
		{
			Intitalize();
			//ToggleMsaa(is4xMsaaEnabled); // Toggle MSAA based on the new value
		}
	}
	ImGui::End();
}

bool Graphics::Intitalize()
{
	HRESULT hr = 0;
	UINT swapDebugFlag = 0u;
	DXGI_MODE_DESC* displayModeList;
	int videoMemory;
	int err;
	unsigned long long stringLength;
	char videoCardDescription[128];


	//graphic interface

	CHECK_RESULT(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)factory.GetAddressOf()));
	CHECK_RESULT(factory->EnumAdapters1(0, adapter.GetAddressOf()));
	CHECK_RESULT(adapter->EnumOutputs(0, adapterOutput.GetAddressOf()));
	CHECK_RESULT(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr));


	//list
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}
	CHECK_RESULT(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList));

	unsigned int i;
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)m_width && displayModeList[i].Height == (unsigned int)m_height)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
			break;
		}
	}

	//desc adapter
	DXGI_ADAPTER_DESC1 adapterDesc = {};
	//ZeroMemory(adapterDesc, sizeof(DXGI_ADAPTER_DESC1));
	CHECK_RESULT(adapter->GetDesc1(&adapterDesc));
	videoMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	//convert name(adapter) to character array and store it
	err = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if (err != 0)
	{
		return false;
	}
	

	//create Device and Contex
#if defined(DEBUG)|| defined(_DEBUG)
	swapDebugFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = { D3D_FEATURE_LEVEL_11_0 };

	CHECK_RESULT(D3D11CreateDevice(
		adapter.Get(),                 // default adapter
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,                 // no software device
		swapDebugFlag,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		pDevice.GetAddressOf(),
		&featureLevel,
		pContext.GetAddressOf()));
	

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}
	CHECK_RESULT(pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4 ,&m4xMsaaQuality
	));
	assert(m4xMsaaQuality > 0);

	

	
	//swapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferCount = 1u;
	if (isVsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//use 4x MSAA
	if (is4xMsaaEnabled)
	{
		OutputDebugString(L"msaa is true");
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		OutputDebugString(L"msaa is false");
	}
	swapChainDesc.Windowed = isFullscreenEnabled;

	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; best perfomance
	
	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; 
	swapChainDesc.Flags = 0;

	CHECK_RESULT(factory->CreateSwapChain(pDevice.Get(), &swapChainDesc, pSwapChain.GetAddressOf()));

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	CHECK_RESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	CHECK_RESULT(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pRenderTarget.GetAddressOf()));

	pBackBuffer.Reset();
	
	//depth stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	// Stencil operations for front-facing pixels
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations for back-facing pixels
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	CHECK_RESULT(pDevice->CreateDepthStencilState(&dsDesc, DSLessEqual.GetAddressOf()));
	// bind depth state
	pContext->OMSetDepthStencilState(DSLessEqual.Get(), 1u);


	//creste the depth class
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = (float)m_width;
	descDepth.Height = (float)m_height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	if (is4xMsaaEnabled)
	{
		descDepth.SampleDesc.Count = 4;
		descDepth.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
	}
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	CHECK_RESULT(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	//create depth view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	if (is4xMsaaEnabled)
	{
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	}
	descDSV.Texture2D.MipSlice = 0u;
	pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, pDSV.GetAddressOf());

	// bind depth stensil view to OM
	pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDSV.Get());


	// configure viewport
	
	vp.Width = (float)m_width;
	vp.Height = (float)m_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	

	delete[] displayModeList;
	displayModeList = 0;
	adapterOutput.Reset();
	adapter.Reset();
	factory.Reset();

	



	return true;
}

void Graphics::Resize()
{

	if (pContext == nullptr || pDevice == nullptr || pSwapChain == nullptr) {
		// Handle null pointer error
		return;
	}

	// Release existing render target and depth stencil views
	pRenderTarget.Reset();
	//pDSV.Reset();
	//pDepthStencil.Reset();

	// Resize the swap chain
	HRESULT hr = pSwapChain->ResizeBuffers(1, m_width, m_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if (FAILED(hr)) {
		// Handle error during ResizeBuffers
		return;
	}

	// Recreate render target view
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer);
	if (FAILED(hr)) {
		// Handle error during GetBuffer
		return;
	}
	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), 0, pRenderTarget.GetAddressOf());
	if (FAILED(hr)) {
		// Handle error during CreateRenderTargetView
		return;
	}
	pBackBuffer.Reset();

	pContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), pDSV.Get());

	
	
	
}
