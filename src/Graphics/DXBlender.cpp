#include "DXBlender.h"
#include "ErrorEx.h"

DXBlender::DXBlender(Microsoft::WRL::ComPtr<ID3D11Device4>& device)
{
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    CHECK_RESULT(device->CreateBlendState(&blendDesc, blendState.GetAddressOf()));
}

DXBlender::~DXBlender()
{
    blendState.Reset();
}

void DXBlender::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context)
{
    const float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    context->OMSetBlendState(blendState.Get(), blendFactor, 0xFFFFFFFF);
}

void DXBlender::Release()
{
    delete this;
}
