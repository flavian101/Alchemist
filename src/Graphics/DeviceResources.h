#pragma once
#include "stdafx.h"
#include "ErrorEx.h"
#include <vector>
#include <string>
#include <codecvt>
#include <memory>



class DeviceResources {
public:
    DeviceResources();
    DeviceResources(const DeviceResources&) = delete;
    DeviceResources& operator=(const DeviceResources&) = delete;
    static DeviceResources* Get();
    void Initialize(HWND hwnd, int width, int height);
    void ReleaseResources();
    ~DeviceResources();

    void Resize(UINT width, UINT height);
    void ToggleMsaa(bool enable);

    ID3D11Device* GetDevice() const { return pDevice.Get(); }
    ID3D11DeviceContext* GetContext() const { return pContext.Get(); }
    IDXGISwapChain* GetSwapChain() const { return pSwapChain.Get(); }
    ID3D11RenderTargetView* GetRenderTargetView() const { return pRenderTarget.Get(); }
    ID3D11DepthStencilView* GetDepthStencilView() const { return pDSV.Get(); }
    ID3D11RasterizerState* GetRasterizerState() const { return CCWcullMode.Get(); }
    ID3D11BlendState* GetBlendState()const { return blendState.Get(); }
    ID3D11RenderTargetView** GetRenderTargetViewAddress() { return pRenderTarget.GetAddressOf(); }

    void BindBlendState();
    UINT GetWidth() const { return m_width; }
    UINT GetHeight() const { return m_height; }
    HWND getHwnd();
    bool isVsyncEnabled;
    bool enableMsaa;

private:
    bool CreateDevice();
    bool CreateSwapChain();
    void CreateDepthStencil(UINT width, UINT height);
    void CreateRenderTarget();
    void CreateRasterizerState();
    void CreateBlendState();
    void InitializeImGui();
    bool EnumerateAdaptersAndOutputs();

    HWND hWnd;
    UINT m_width;
    UINT m_height;
    bool isFullscreenEnabled = true;

    Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
    Microsoft::WRL::ComPtr< IDXGIDevice1> pDDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> CCWcullMode = nullptr;
    Microsoft::WRL::ComPtr<IDXGIFactory1> pFactory = nullptr;
    Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter = nullptr;
    Microsoft::WRL::ComPtr<IDXGIOutput> adapterOutput = nullptr;
    Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilState = nullptr;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState = nullptr;

    DXGI_MODE_DESC* displayModeList;
    UINT numerator;
    UINT denominator;
    unsigned int numModes;
    UINT m4xMsaaQuality;
    int err;
    int videoMemory;
    char videoCardDescription[128];

    //friend classes
    friend class SceneSerializer;
};
