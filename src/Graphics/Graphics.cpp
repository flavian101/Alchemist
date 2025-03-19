#include "Graphics.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "DXDevice.h"
#include "DXContext.h"



Graphics::Graphics(HWND hwnd,UINT width, UINT height)
    :
    imguiEnabled(true),
    renderer(std::make_unique<Renderer>(hwnd,width,height)),
    view(XMMatrixIdentity()),
    projection(XMMatrixIdentity())
{
    renderer->initializeImgui();
}

Graphics::~Graphics()
{}


void Graphics::ClearDepthColor(float red, float green, float blue) {
    renderer->Bind(red, green, blue, 1.0f);
}

void Graphics::Render(UINT indexCount) {
    //Renderer::Get()->BindBlendState();
    renderer->Render(indexCount);
}

void Graphics::End()
{
    renderer->EndFrame();
}

void Graphics::ControlWindow()
{
    ImGui::Text("Graphics Settings");
    ImGui::Text("Resolution:%.2f X %.2f ", GetWidth(), GetHeight());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
   // ImGui::Checkbox("Vsync", &Renderer::Get()->isVsyncEnabled);
   // bool msaaChanged = ImGui::Checkbox("MSAA", &Renderer::Get()->enableMsaa);
   // if (msaaChanged) {
   //     Renderer::Get()->ToggleMsaa(Renderer::Get()->enableMsaa);
   // }
}

Microsoft::WRL::ComPtr<ID3D11Device4> Graphics::GetDevice()
{
    return renderer->device->getDevice();
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext4> Graphics::GetContext()
{
    return renderer->context->getContext();
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
    return static_cast<float>(renderer->GetWidth()) / renderer->GetHeight();
}

float Graphics::GetWidth() const {
    return  renderer->GetWidth();
}

float Graphics::GetHeight() const {
    return  renderer->GetHeight();
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




