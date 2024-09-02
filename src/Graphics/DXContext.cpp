#include "DXContext.h"
#include "DXDevice.h"
#include "ErrorEx.h"

DXContext::DXContext(DXDevice& device)
{
    Microsoft::WRL::ComPtr<ID3D11DeviceContext3>pContext3;

    device.getDevice()->GetImmediateContext3(pContext3.GetAddressOf());
    CHECK_RESULT(pContext3->QueryInterface(__uuidof(ID3D11DeviceContext4), reinterpret_cast<void**>(pContext.GetAddressOf())));
    pContext3.Reset();
    if (!pContext) {
        std::cerr << "Failed to get device context" << std::endl;
    }
}

DXContext::~DXContext()
{
    pContext.Reset();
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& DXContext::getContext()
{
    return pContext;
}


