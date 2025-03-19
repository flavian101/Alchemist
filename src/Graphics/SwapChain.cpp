#include "SwapChain.h"
#include "ErrorEx.h"
#include "DXGIAdapterInfo.h"
#include "DXDevice.h"
#include "DXContext.h"
#include "DXRenderTarget.h"

SwapChain::SwapChain(HWND hwnd, DXGIAdapterInfo& info, DXDevice& device,UINT width, UINT height, bool enableMsaa)
{

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //to implement vsync later
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = enableMsaa ? 4 : 1;
    swapChainDesc.SampleDesc.Quality = enableMsaa ? device.getMsaaLevel() - 1 : 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


    CHECK_RESULT(info.pFactory->CreateSwapChain(
        device.pDevice.Get(), &swapChainDesc,pSwapChain.GetAddressOf()));

    //resize the swap chain to the highest res of the primary adapter
    //info.SetHighestResolution(device.pDevice, pSwapChain, hwnd);

   
    ///create the render target after swapchain initialization
}

SwapChain::~SwapChain()
{

}

void SwapChain::Resize(UINT width, UINT height)
{
}



void SwapChain::Present(bool isVsyncEnabled)
{
    pSwapChain->Present(isVsyncEnabled ?1u:0u ,0u );
}



Microsoft::WRL::ComPtr<IDXGISwapChain>& SwapChain::getIDXGISwapChain()
{
    return pSwapChain;
}
