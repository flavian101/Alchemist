#include "DXDevice.h"
#include "ErrorEx.h"
#include "DXContext.h"
#include "DXGIAdapterInfo.h"


DXDevice::DXDevice(DXGIAdapterInfo& info)
    :
    pDevice(nullptr)
{
    //
   // created a null context to see if it works with a null context then getting the immediate context in the DXContext class 
   //
    UINT creationFlags = 0u;
#if defined(DEBUG) || defined(_DEBUG)
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL featureLevel = { D3D_FEATURE_LEVEL_11_0 };


    Microsoft::WRL::ComPtr<ID3D11Device> pDevice1 = nullptr;

    //Microsoft::WRL::ComPtr<ID3D11DeviceContext>pContext1;
    //set to default nullptr adapter for debugging later(info.pAdapter.Get(),)
    CHECK_RESULT(D3D11CreateDevice(
        info.pAdapter.Get(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,                 //software device
        creationFlags,
        nullptr,
        0,              // default feature level array
        D3D11_SDK_VERSION,
        pDevice1.GetAddressOf(),
        &featureLevel,
        nullptr));

    if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
        MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
    }
    CHECK_RESULT(pDevice1->QueryInterface(__uuidof(ID3D11Device4), reinterpret_cast<void**>(pDevice.GetAddressOf())));
    // CHECK_RESULT(pContext1->QueryInterface(__uuidof(ID3D11DeviceContext4), reinterpret_cast<void**>(pImmediateContext.GetAddressOf())));

     //release the original interfaces
    pDevice1.Reset();
    //  pContext1.Reset();

    //release the original interfaces
    pDevice1.Reset();
    //pContext1.Reset();
}

DXDevice::~DXDevice()
{
    pDevice.Reset();
}

void DXDevice::CheckMsaaLevel()
{
    CHECK_RESULT(pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
    assert(m4xMsaaQuality > 0);
}


Microsoft::WRL::ComPtr<ID3D11Device4>& DXDevice::getDevice()
{
    return pDevice;
}

UINT DXDevice::getMsaaLevel()
{
    return m4xMsaaQuality;
}
