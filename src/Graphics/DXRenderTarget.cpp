#include "DXRenderTarget.h"
#include "DXDevice.h"
#include "DXContext.h"
#include "ErrorEx.h"
#include "DXDepthStencil.h"

DXRenderTarget::DXRenderTarget(Microsoft::WRL::ComPtr<ID3D11Device4>& device, Microsoft::WRL::ComPtr<IDXGISwapChain>& swapchain, bool enableMsaa)
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backbuffer;

    CHECK_RESULT(swapchain->GetBuffer(0u, __uuidof(ID3D11Resource), reinterpret_cast<void**>(backbuffer.GetAddressOf())));
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    rtvDesc.ViewDimension = enableMsaa ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;

    CHECK_RESULT(device->CreateRenderTargetView(backbuffer.Get(), &rtvDesc, pRenderTarget.GetAddressOf()));
    backbuffer.Reset();
}

DXRenderTarget::~DXRenderTarget()
{
    pRenderTarget.Reset();

}

bool DXRenderTarget::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context,DXDepthStencil& view)
{
    //ID3D11RenderTargetView* pRtv = pRenderTarget.Get();
    context->ClearDepthStencilView(view.getDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    context->OMSetRenderTargets(1, pRenderTarget.GetAddressOf(), view.getDSV().Get());
  //  pRtv->Release();
    return true;
}

bool DXRenderTarget::ClearDepthAndColor(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context, float red, float blue, float green, float alpha)
{

    const float color[] = { red, blue,green, alpha };
    context->ClearRenderTargetView(pRenderTarget.Get(), color);
    return true;
}


