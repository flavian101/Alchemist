#include "Renderer.h"
#include "imgui/imgui.h"
#include "DXGIAdapterInfo.h"
#include "DXDevice.h"
#include "DXContext.h"
#include "DXRenderTarget.h"
#include "DXBlender.h"
#include "DXRasterizer.h" 
#include "DXDepthStencil.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "SwapChain.h"
#include "DXViewPort.h"



Renderer::Renderer(HWND hwnd)
    :
    hWnd(hwnd),
    m_width(1024.0f),
    m_height(720.0f),
    enabledMsaa(false),
    isVsyncEnabled(false)
{
    info = std::make_unique<DXGIAdapterInfo>();
    EnumarateAdapters();

    device = std::make_unique<DXDevice>(*info.get());

    swapChain = std::make_unique<SwapChain>(hWnd, *info.get(), *device.get(), enabledMsaa);
    context = std::make_unique<DXContext>(*device);
    m_width = info->getWidth();
    m_height = info->getHeight();
    renderTarget = std::make_unique<DXRenderTarget>(device->getDevice(), swapChain->getIDXGISwapChain(), enabledMsaa);
    depthStencil = std::make_unique<DXDepthStencil>(*device, m_width, m_height, enabledMsaa);
    rasterizer = std::make_unique<DXRasterizer>(device->getDevice());

  
    viewPort = std::make_unique<DXViewPort>(m_width, m_height);
    viewPort->Bind(context->getContext());
    
    createDevice();
    createContext();
    createSwapChain();
    createDepthStencil();
    CreateRenderTarget();
    createRasterizer();
    CreateViewPort();
}

Renderer::~Renderer()
{
    ImGui_ImplDX11_Shutdown();
    device->~DXDevice();
    context->~DXContext();
    swapChain->~SwapChain();
    renderTarget->~DXRenderTarget();
    depthStencil->~DXDepthStencil();
    rasterizer->~DXRasterizer();
}

bool Renderer::EnumarateAdapters()
{
    info->PrintAdapterInfo();
  
    return true;
}

bool Renderer::createDevice()
{
    return true;
}
bool Renderer::createContext()
{
    return true;
}

bool Renderer::createSwapChain()
{
    return true;
}

bool Renderer::createDepthStencil()
{
    
    return true;
}

bool Renderer::CreateRenderTarget()
{
    return true;
}

bool Renderer::createRasterizer()
{
    return true;
}

bool Renderer::CreateViewPort()
{
    return false;
}

bool Renderer::initializeImgui()
{
    return ImGui_ImplDX11_Init(device->getDevice().Get(), context->getContext().Get());
}

void Renderer::Bind(float red, float green, float blue, float alpha)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    renderTarget->ClearDepthAndColor(context->getContext(), red, green, blue, alpha);
    depthStencil->Bind(context->getContext());
    renderTarget->Bind(context->getContext(), *depthStencil.get());
    rasterizer->Bind(context->getContext());

}

void Renderer::Render(UINT indexCount)
{
    context->getContext()->DrawIndexed(indexCount, 0, 0);
}

void Renderer::EndFrame()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    swapChain->Present(isVsyncEnabled);
}

UINT Renderer::GetWidth()const
{
    return m_width;
}

UINT Renderer::GetHeight()const
{
    return m_height;
}








 