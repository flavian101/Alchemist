#include "Graphics.h"
#include "DeviceResources.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

Graphics::Graphics()
    :
    imguiEnabled(true) 
{

}

void Graphics::Initialize(HWND hwnd, int width, int height)
{
    DeviceResources::Get()->Initialize(hwnd, width, height);
}

Graphics& Graphics::GetInstance()
{
    static Graphics graphics;
    return graphics;
}

void Graphics::DestroyInstance()
{
    DeviceResources::Get()->ReleaseResources();
}

Graphics::~Graphics()
{
}

DeviceResources* Graphics::GetDeviceResources()
{
    return DeviceResources::Get();
}

void Graphics::ClearDepthColor(float red, float green, float blue) {
    if (imguiEnabled)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
    const float color[] = { red, green, blue, 1.0f };

    DeviceResources::Get()->GetContext()->ClearRenderTargetView(DeviceResources::Get()->GetRenderTargetView(), color);
    DeviceResources::Get()->GetContext()->ClearDepthStencilView(DeviceResources::Get()->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    DeviceResources::Get()->GetContext()->OMSetRenderTargets(1, DeviceResources::Get()->GetRenderTargetViewAddress(), DeviceResources::Get()->GetDepthStencilView());
}

void Graphics::Render(UINT indexCount) {
    DeviceResources::Get()->BindBlendState();
    DeviceResources::Get()->GetContext()->RSSetState(DeviceResources::Get()->GetRasterizerState());
    DeviceResources::Get()->GetContext()->DrawIndexed(indexCount, 0, 0);
}

void Graphics::End() {
    if (imguiEnabled) {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
   CHECK_RESULT(DeviceResources::Get()->GetSwapChain()->Present(DeviceResources::Get()->isVsyncEnabled ? 1 : 0, 0));
}

void Graphics::ControlWindow()
{
    ImGui::Text("Graphics Settings");
    ImGui::Text("Resolution:%.2f X %.2f ", GetWidth(), GetHeight());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Checkbox("Vsync", &DeviceResources::Get()->isVsyncEnabled);
    bool msaaChanged = ImGui::Checkbox("MSAA", &DeviceResources::Get()->enableMsaa);
    if (msaaChanged) {
        DeviceResources::Get()->ToggleMsaa(DeviceResources::Get()->enableMsaa);
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
    return static_cast<float>(DeviceResources::Get()->GetWidth()) / DeviceResources::Get()->GetHeight();
}

float Graphics::GetWidth() const {
    return  DeviceResources::Get()->GetWidth();
}

float Graphics::GetHeight() const {
    return  DeviceResources::Get()->GetHeight();
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


