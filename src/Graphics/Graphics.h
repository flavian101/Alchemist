#pragma once
#include "DeviceResources.h"
#include <tuple>


class Graphics {
public:
    Graphics(HWND hwnd, int width, int height);
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    ~Graphics();

    DeviceResources* GetDeviceResources() const;

    void ClearDepthColor(float red, float green, float blue);
    void Render(UINT indexCount);
    void End();
    void ControlWindow();

    void EnableImgui();
    void DisableImgui();
    bool IsImguiEnabled() const noexcept;

    void SetProjectionMatrix(DirectX::FXMMATRIX projectionMatrix);
    DirectX::XMMATRIX GetProjectionMatrix() const;

    void SetViewMatrix(DirectX::FXMMATRIX viewMatrix);
    DirectX::XMMATRIX GetViewMatrix() const;

    float GetAspectRatio() const;
    float GetWidth() const;
    float GetHeight() const;
    //
    void SetWin(const std::tuple<HWND, HINSTANCE>& win);
    std::tuple<HWND, HINSTANCE> getWin();

private:
    std::unique_ptr<DeviceResources> deviceResources;
    HWND m_hwnd;
    HINSTANCE m_hinstance;
    DirectX::XMMATRIX projection;
    DirectX::XMMATRIX view;

    bool imguiEnabled ;
    friend class SceneSerializer;
};
