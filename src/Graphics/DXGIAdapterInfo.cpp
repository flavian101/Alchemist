#include "DXGIAdapterInfo.h"
#include "ErrorEx.h"

DXGIAdapterInfo::DXGIAdapterInfo()
    :
    pFactory(nullptr),
    pAdapter(nullptr),
    pDDevice(nullptr),
    m_bestmode({})
{
    UINT DebugFlag = 0u;
#if defined(DEBUG) || defined(_DEBUG)
    DebugFlag |= DXGI_CREATE_FACTORY_DEBUG;
#endif
    CHECK_RESULT(CreateDXGIFactory2(0u, __uuidof(IDXGIFactory2), (void**)pFactory.GetAddressOf()));

    m_adapters = EnumerateAdapters();
    if (m_adapters.empty()) {
        std::cerr << "Graphics card  found" << std::endl;
    }
    choosePrimaryAdapter();

}

DXGIAdapterInfo::~DXGIAdapterInfo()
{
    for (auto adapter : m_adapters) {
        if (adapter) adapter->Release();
    }
    if (pAdapter) pAdapter.Reset();

    if (pFactory) pFactory.Reset();
    pDDevice.Reset();
}



void DXGIAdapterInfo::PrintAdapterInfo()
{
    for (size_t i = 0; i < m_adapters.size(); ++i) {
        DXGI_ADAPTER_DESC1 desc;
        m_adapters[i]->GetDesc1(&desc);
        std::wcout << L"Adapter " << i << L": " << desc.Description << std::endl;
    }
}

void DXGIAdapterInfo::SetHighestResolution(Microsoft::WRL::ComPtr<ID3D11Device4>& device, Microsoft::WRL::ComPtr<IDXGISwapChain>& swapChain, HWND hwnd)
{
    Microsoft::WRL::ComPtr<IDXGIOutput1> output = nullptr;
    for (UINT i = 0; pAdapter->EnumOutputs(i, reinterpret_cast<IDXGIOutput**>(output.GetAddressOf())) != DXGI_ERROR_NOT_FOUND; ++i) {
        DXGI_MODE_DESC bestMode = GetHighestResolution(pAdapter, output);

        // Ensure valid mode was found
        if (bestMode.Width != 0 && bestMode.Height != 0) {
            m_bestmode = bestMode;

            // Adjust the swap chain description with the best mode found
            DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
            swapChain->GetDesc(&swapChainDesc);
            swapChainDesc.BufferDesc.Width = m_bestmode.Width;
            swapChainDesc.BufferDesc.Height = m_bestmode.Height;

            //CHECK_RESULT(swapChain->ResizeTarget(&swapChainDesc.BufferDesc));
            swapChain->ResizeBuffers(swapChainDesc.BufferCount, m_bestmode.Width, m_bestmode.Height, swapChainDesc.BufferDesc.Format, swapChainDesc.Flags);

            break;  // Exit after setting the best mode
        }
    }

    if (m_bestmode.Width == 0 || m_bestmode.Height == 0) {
        std::cerr << "Failed to find a valid resolution." << std::endl;
    }
  
}

void DXGIAdapterInfo::choosePrimaryAdapter()
{
    for (auto adapter : m_adapters) {
        UINT numOutputs = 0;

        IDXGIOutput* output = nullptr;
        for (UINT i = 0; adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND; ++i) {
            if (isPrimaryAdapter(output)) {
                if (pAdapter) pAdapter.Reset();
                pAdapter = adapter; // Set the primary adapter
                output->Release();  // Release the output as it's no longer needed
                return;
            }
            output->Release();
        }
    }

    // If no primary adapter is found, default to the first adapter
    if (pAdapter) pAdapter.Reset();
    pAdapter = m_adapters[0];
}

UINT DXGIAdapterInfo::getHeight()
{
    return m_bestmode.Height;
}

UINT DXGIAdapterInfo::getWidth()
{
    return m_bestmode.Width;
}

std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> DXGIAdapterInfo::EnumerateAdapters()
{
    std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> adapters;
    Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter = nullptr;
    for (UINT i = 0; pFactory->EnumAdapters1(i, adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i) {
        adapters.push_back(adapter);
    }
    return adapters;
}

DXGI_MODE_DESC DXGIAdapterInfo::GetHighestResolution(Microsoft::WRL::ComPtr<IDXGIAdapter1>& adapter, Microsoft::WRL::ComPtr<IDXGIOutput1>& output)
{
    DXGI_MODE_DESC bestMode = {};
    UINT numModes = 0;
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

    output->GetDisplayModeList(format, 0, &numModes, nullptr);
    std::vector<DXGI_MODE_DESC> modes(numModes);
    output->GetDisplayModeList(format, 0, &numModes, modes.data());

    for (const auto& mode : modes) {
        if (mode.Width > bestMode.Width || (mode.Width == bestMode.Width && mode.Height > bestMode.Height)) {
            bestMode = mode;
        }
    }
    return bestMode;
}

bool DXGIAdapterInfo::isPrimaryAdapter(Microsoft::WRL::ComPtr<IDXGIOutput> output)
{
    DXGI_OUTPUT_DESC outputDesc;
    output->GetDesc(&outputDesc);

    DISPLAY_DEVICE displayDevice = {};
    displayDevice.cb = sizeof(displayDevice);
    for (DWORD i = 0; EnumDisplayDevices(nullptr, i, &displayDevice, 0); ++i) {
        if (displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
            std::wstring deviceName = displayDevice.DeviceName;
            if (deviceName == outputDesc.DeviceName) {
                return true;
            }
        }
    }
    return false;
}
