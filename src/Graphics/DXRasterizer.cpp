#include "DXRasterizer.h"
#include "ErrorEx.h"

DXRasterizer::DXRasterizer(Microsoft::WRL::ComPtr<ID3D11Device4>& device)
    :
    CCWcullMode(nullptr)
{
    D3D11_RASTERIZER_DESC1 rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FrontCounterClockwise = FALSE;
    rasterDesc.DepthClipEnable = TRUE;
    //rasterDesc.MultisampleEnable = enableMsaa;
    CHECK_RESULT(device->CreateRasterizerState1(&rasterDesc, CCWcullMode.GetAddressOf()));

}

DXRasterizer::~DXRasterizer()
{
    CCWcullMode.Reset();
}

void DXRasterizer::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context)
{
    context->RSSetState(CCWcullMode.Get());
}


