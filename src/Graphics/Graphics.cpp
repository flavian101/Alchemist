#include "Graphics.h"
#include <cassert>
#include "ErrorEx.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

Graphics::Graphics(HWND hwnd, int Width, int Height, bool FullScreen)
	:
	hWnd(hwnd),
	m_width(Width),
	m_height(Height)
{
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));

	HRESULT hr = 0;
	DXGI_MODE_DESC* displayModeList;
	int videoMemory;
	int err;
	unsigned long long stringLength;
	char videoCardDescription[128];

	//graphic interface
	CHECK_RESULT(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)factory.GetAddressOf()));
	CHECK_RESULT(factory->EnumAdapters1(0u, adapter.GetAddressOf()));
	CHECK_RESULT(adapter->EnumOutputs(0u, adapterOutput.GetAddressOf()));
	CHECK_RESULT(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr));


	//list
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		//return false;
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
		//return false;
	}

	delete[] displayModeList;
	displayModeList = 0;
	
	if (!CreateDevice())
	{
		MessageBox(hWnd, L"failed to create device", L"Error", MB_ICONEXCLAMATION);
	}

	ImGui_ImplDX11_Init(GetDevice().Get(), GetContext().Get());
	
	if (!Intitalize())
	{
		MessageBox(hWnd,L"failed to initialize swapchain",L"Error", MB_ICONEXCLAMATION);
	}

}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	pSwapChain.Reset();
	adapterOutput.Reset();
	device.Reset();
	adapter.Reset();
	factory.Reset();
	
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
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

	//Set our Render Target
	pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDSV.Get());


}

void Graphics::Render(UINT indexCount)
{
	pContext->RSSetState(CCWcullMode.Get());
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

void Graphics::controlWindow()
{
	if (ImGui::Begin("Graphics Settings"))
	{
		//ImGui::SliderFloat("Speed Factor", &speedfactor, 0.0f, 6.0f, "%.4f", 3.2f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("Vsync",& isVsyncEnabled);
		bool msaaChanged = ImGui::Checkbox("Msaa", &enableMsaa);
		// Check if MSAA checkbox value changed
		if (msaaChanged)
		{
			//Intitalize();
			ToggleMsaa(enableMsaa); // Toggle MSAA based on the new value
		}

	

	}
	ImGui::End();
}


bool Graphics::Intitalize()
{
	//swapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferCount = 1u;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
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


	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//use 4x MSAA
	if (enableMsaa)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	swapChainDesc.Windowed = isFullscreenEnabled;
	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; best perfomance
	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; 
	swapChainDesc.Flags = 0;
	CHECK_RESULT(factory->CreateSwapChain(pDevice.Get(), &swapChainDesc, pSwapChain.GetAddressOf()));
	
	
	Resize(m_width,m_height);

	


	return true;
}
bool Graphics::CreateDevice()
{
	UINT swapDebugFlag = 0u;

	//create Device and Contex
#if defined(DEBUG)|| defined(_DEBUG)
	swapDebugFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL featureLevel = { D3D_FEATURE_LEVEL_11_0 };

	CHECK_RESULT(D3D11CreateDevice(
		adapter.Get(),
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
		//return false;
	}
	CHECK_RESULT(pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality
	));
	assert(m4xMsaaQuality > 0);

	return true;
}
bool Graphics::DepthStencil()
{
	//depth stencil
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
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
	
	return true;
}

void Graphics::Resize(UINT width, UINT height)
{
	DepthStencil();
	assert(pContext);
	assert(pDevice);
	assert(pSwapChain);

	// Release existing render target and depth stencil views
	pRenderTarget.Reset();
	//pDSV.Reset();
	//pDepthStencil.Reset();
	
	
	// Resize the swap chain
	CHECK_RESULT(pSwapChain->ResizeBuffers(1u, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	// Recreate render target view
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	CHECK_RESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	CHECK_RESULT(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pRenderTarget.GetAddressOf()));
	pBackBuffer.Reset();

	//create the depth class
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = (float)width;
	descDepth.Height = (float)height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (enableMsaa)
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
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	CHECK_RESULT(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	//create depth view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (enableMsaa)
	{
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	}
	descDSV.Texture2D.MipSlice = 0u;

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, pDSV.GetAddressOf());
	// bind depth stensil view to OM
	pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDSV.Get());


	// configure viewport
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	D3D11_RASTERIZER_DESC rDesc;
	ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.CullMode = D3D11_CULL_NONE;
	rDesc.FrontCounterClockwise = false;
	rDesc.CullMode = D3D11_CULL_NONE;
	CHECK_RESULT(pDevice->CreateRasterizerState(&rDesc, CCWcullMode.GetAddressOf()));

	
}


void Graphics::ToggleMsaa(bool enableMsaa)
{

	enableMsaa = enableMsaa;
	pRenderTarget.Reset();
	pDSV.Reset();
	pDepthStencil.Reset();
	pSwapChain.Reset();
	pDepthStencil.Reset();
	DSLessEqual.Reset();
	CCWcullMode.Reset();
	Intitalize();
	
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


void Graphics::SetProjectionMatrix(DirectX::FXMMATRIX projectionMatrix)
{
	projection = projectionMatrix;
}

DirectX::XMMATRIX Graphics::GetProjectionMatrix() const
{
	return projection;
}

void Graphics::SetViewMatrix(DirectX::FXMMATRIX viewMatrix)
{
	view = viewMatrix;
}

DirectX::XMMATRIX Graphics::GetViewMatrix() const
{
	return view;
}


