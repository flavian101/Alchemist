#include "DXViewPort.h"

DXViewPort::DXViewPort(UINT Width, UINT height)
:
    viewport({})
{
    viewport.Width = static_cast<float>(Width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
}

DXViewPort::~DXViewPort()
{
}

bool DXViewPort::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context)
{
    context->RSSetViewports(1u, &viewport);
    return true;
}
