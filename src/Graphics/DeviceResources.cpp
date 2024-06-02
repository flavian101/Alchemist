#include "DeviceResources.h"

DeviceResources::DeviceResources(HWND hwnd, int width, int height)
    : hWnd(hwnd), m_width(width), m_height(height),
    isFullscreenEnabled(true),
    isVsyncEnabled(true),
    enableMsaa(false) {
    Initialize();
}

DeviceResources::~DeviceResources() {
    ImGui_ImplDX11_Shutdown();
    adapterOutput.Reset();
    adapter.Reset();
    factory.Reset();
    pSwapChain.Reset();
    pDevice.Reset();
    pContext.Reset();
    pRenderTarget.Reset();
    pDepthStencil.Reset();
    pDSV.Reset();
    CCWcullMode.Reset();
}

void DeviceResources::Initialize() {
    EnumerateAdaptersAndOutputs();
    if (!CreateDevice()) {
        MessageBox(hWnd, L"Failed to create device", L"Error", MB_ICONEXCLAMATION);
    }
    if (!CreateSwapChain()) {
        MessageBox(hWnd, L"Failed to create swap chain", L"Error", MB_ICONEXCLAMATION);
    }
    InitializeImGui();
  
    CreateDepthStencil(m_width, m_height);
    CreateRenderTarget();
    CreateRasterizerState();

}

bool DeviceResources::CreateDevice() {
    UINT creationFlags = 0u;
#if defined(DEBUG) || defined(_DEBUG)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL featureLevel = { D3D_FEATURE_LEVEL_11_0 };

    CHECK_RESULT(D3D11CreateDevice(
        adapter.Get(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,                 //software device
        creationFlags,
        nullptr,
        0,              // default feature level array
        D3D11_SDK_VERSION,
        pDevice.GetAddressOf(),
        &featureLevel,
        pContext.GetAddressOf()));

    if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
        MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
        return false;
    }
    CHECK_RESULT(pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
    assert(m4xMsaaQuality > 0);
    return true;
}

bool DeviceResources::CreateSwapChain() {
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferDesc.Width = m_width;
    swapChainDesc.BufferDesc.Height = m_height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = isVsyncEnabled ? numerator : 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = isVsyncEnabled ? denominator : 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = isFullscreenEnabled;
    swapChainDesc.SampleDesc.Count = enableMsaa ? 4 : 1;
    swapChainDesc.SampleDesc.Quality = enableMsaa ? m4xMsaaQuality - 1 : 0;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;
    CHECK_RESULT(factory->CreateSwapChain(pDevice.Get(), &swapChainDesc, pSwapChain.GetAddressOf()));
    return true;
}

void DeviceResources::CreateDepthStencil(UINT width, UINT height) {
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = enableMsaa ? 4 : 1;
    descDepth.SampleDesc.Quality = enableMsaa ? m4xMsaaQuality - 1 : 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    CHECK_RESULT(pDevice->CreateTexture2D(&descDepth, nullptr, pDepthStencil.GetAddressOf()));

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSV.ViewDimension = enableMsaa ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    CHECK_RESULT(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, pDSV.GetAddressOf()));
    pContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), pDSV.Get());
}

void DeviceResources::CreateRenderTarget() 
{
    pRenderTarget.Reset();
    Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
    CHECK_RESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(pBackBuffer.GetAddressOf())));

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvDesc.ViewDimension = enableMsaa ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

    CHECK_RESULT(pDevice->CreateRenderTargetView(pBackBuffer.Get(),  nullptr, pRenderTarget.GetAddressOf()));
    pContext->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), pDSV.Get());



    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(m_width);
    viewport.Height = static_cast<float>(m_height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    pContext->RSSetViewports(1, &viewport);
}

void DeviceResources::CreateRasterizerState() {
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = FALSE;
    rasterDesc.DepthClipEnable = TRUE;
    rasterDesc.MultisampleEnable = enableMsaa;
    CHECK_RESULT(pDevice->CreateRasterizerState(&rasterDesc, CCWcullMode.GetAddressOf()));
    pContext->RSSetState(CCWcullMode.Get());
}

void DeviceResources::InitializeImGui() {
    ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

void DeviceResources::EnumerateAdaptersAndOutputs() {
    unsigned long long stringLength;

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


    delete[] displayModeList;
    displayModeList = 0;
}

void DeviceResources::Resize(UINT width, UINT height) {
    m_width = width;
    m_height = height;

    pRenderTarget.Reset();
    pDSV.Reset();
    pDepthStencil.Reset();

    CreateSwapChain();
    CHECK_RESULT(pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
    CreateDepthStencil(width, height);
    CreateRenderTarget();
}

void DeviceResources::ToggleMsaa(bool enable) {
    enableMsaa = enable;
    Resize(m_width, m_height);

}
