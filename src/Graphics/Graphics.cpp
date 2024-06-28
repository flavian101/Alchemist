#include "Graphics.h"

Graphics::Graphics(HWND hwnd, int width, int height)
    : deviceResources(std::make_unique<DeviceResources>(hwnd, width, height)),
    imguiEnabled(true) {}

Graphics::~Graphics() {}

DeviceResources* Graphics::GetDeviceResources() const
{
    return deviceResources.get();
}

void Graphics::ClearDepthColor(float red, float green, float blue) {
    if (imguiEnabled)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
    const float color[] = { red, green, blue, 1.0f };
    deviceResources->GetContext()->ClearRenderTargetView(deviceResources->GetRenderTargetView(), color);
    deviceResources->GetContext()->ClearDepthStencilView(deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    deviceResources->GetContext()->OMSetRenderTargets(1, deviceResources->GetRenderTargetViewAddress(), deviceResources->GetDepthStencilView());
}

void Graphics::Render(UINT indexCount) {
    deviceResources->GetContext()->RSSetState(deviceResources->GetRasterizerState());
    deviceResources->GetContext()->DrawIndexed(indexCount, 0, 0);
}

void Graphics::End() {
    if (imguiEnabled) {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
    deviceResources->GetSwapChain()->Present(deviceResources->isVsyncEnabled ? 1 : 0, 0);
}

void Graphics::ControlWindow()
{
    ImGui::Text("Graphics Settings");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Vsync", &deviceResources->isVsyncEnabled);
    bool msaaChanged = ImGui::Checkbox("MSAA", &deviceResources->enableMsaa);
    if (msaaChanged) {
       deviceResources->ToggleMsaa(deviceResources->enableMsaa);
    }
}

void Graphics::EnableImgui() {
    imguiEnabled = true;
}

void Graphics::DisableImgui() {
    imguiEnabled = false;
}

bool Graphics::IsImguiEnabled() const noexcept {
    return imguiEnabled;
}

void Graphics::SetProjectionMatrix(DirectX::FXMMATRIX projectionMatrix) {
    projection = projectionMatrix;
}

DirectX::XMMATRIX Graphics::GetProjectionMatrix() const {
    return projection;
}

void Graphics::SetViewMatrix(DirectX::FXMMATRIX viewMatrix) {
    view = viewMatrix;
}

DirectX::XMMATRIX Graphics::GetViewMatrix() const {
    return view;
}

float Graphics::GetAspectRatio() const {
    return static_cast<float>(deviceResources->GetWidth()) / deviceResources->GetHeight();
}

float Graphics::GetWidth() const {
    return deviceResources->GetWidth();
}

float Graphics::GetHeight() const {
    return deviceResources->GetHeight();
}

void Graphics::SetWin(const std::tuple<HWND, HINSTANCE>& win)
{
    m_hwnd = std::get<0>(win);
    m_hinstance = std::get<1>(win);
}

std::tuple<HWND, HINSTANCE> Graphics::getWin()
{
    return std::make_tuple(m_hwnd,m_hinstance);
}


