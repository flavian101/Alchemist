#include "DXDepthStencil.h"
#include "DXDevice.h"
#include "DXGIAdapterInfo.h"
#include "ErrorEx.h"

DXDepthStencil::DXDepthStencil(DXDevice& device, UINT width, UINT height, bool enableMsaa)
    :
    pDepthState(nullptr),
    pDSV(nullptr)
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = false;
    CHECK_RESULT(device.getDevice()->CreateDepthStencilState(&depthStencilDesc, pDepthState.GetAddressOf()));

    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;

    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = enableMsaa ? 4 : 1;
    descDepth.SampleDesc.Quality = enableMsaa ? device.getMsaaLevel() - 1 : 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    CHECK_RESULT(device.getDevice()->CreateTexture2D(&descDepth, nullptr, pDepthStencil.GetAddressOf()));

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSV.ViewDimension = enableMsaa ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    CHECK_RESULT(device.getDevice()->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, pDSV.GetAddressOf()));

}

DXDepthStencil::~DXDepthStencil()
{
    pDepthState.Reset();
    pDSV.Reset();
}

void DXDepthStencil::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context)
{
    context->OMSetDepthStencilState(pDepthState.Get(), 0u);
    ///the stencil view bound together with the render target
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& DXDepthStencil::getDSV()
{
    return pDSV;
}
