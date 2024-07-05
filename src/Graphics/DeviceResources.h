#pragma once
#include "ErrorEx.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <string>
#include <DirectXMath.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <codecvt>
#include <memory>



class DeviceResources {
public:
    DeviceResources(HWND hwnd, int width, int height);
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
    void Initialize();
    bool CreateDevice();
    bool CreateSwapChain();
    void CreateDepthStencil(UINT width, UINT height);
    void CreateRenderTarget();
    void CreateRasterizerState();
    void CreateBlendState();
    void InitializeImGui();
    void EnumerateAdaptersAndOutputs();

    HWND hWnd;
    UINT m_width;
    UINT m_height;
    bool isFullscreenEnabled = true;

    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> CCWcullMode;
    Microsoft::WRL::ComPtr<IDXGIFactory1> factory;
    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
    Microsoft::WRL::ComPtr<IDXGIOutput> adapterOutput;
    Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilState;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

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
