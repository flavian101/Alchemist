#pragma once
#include "DeviceResources.h"


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

private:
    std::unique_ptr<DeviceResources> deviceResources;

    DirectX::XMMATRIX projection;
    DirectX::XMMATRIX view;

    bool imguiEnabled ;
    friend class SceneSerializer;
};
