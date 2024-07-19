#pragma once
#include <tuple>
#include <memory>
#include "stdafx.h"

class SceneSerializer;
class DeviceResources;

class Graphics {
public:

    Graphics();
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
    void Initialize(HWND hwnd, int width, int height);

    static Graphics& GetInstance();
    static void DestroyInstance();
    ~Graphics();

    static DeviceResources* GetDeviceResources();

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
    HWND m_hwnd;
    HINSTANCE m_hinstance;
    DirectX::XMMATRIX projection;
    DirectX::XMMATRIX view;

    bool imguiEnabled ;
    friend class SceneSerializer;
};
