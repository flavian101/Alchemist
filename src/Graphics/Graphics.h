#pragma once
#include <tuple>
#include <memory>
#include "stdafx.h"

class SceneSerializer;
class Renderer;

class Graphics {
private:
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;
public:
    Graphics(HWND hwnd);
   // Graphics();
    ~Graphics();

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

    //To be implementes as static methods
     Microsoft::WRL::ComPtr< ID3D11Device4> GetDevice();
     Microsoft::WRL::ComPtr<ID3D11DeviceContext4> GetContext();



private:
    HWND m_hwnd;
    HINSTANCE m_hinstance;
    DirectX::XMMATRIX projection;
    DirectX::XMMATRIX view;
    bool imguiEnabled ;
    std::unique_ptr<Renderer> renderer;

private:
    friend class SceneSerializer;
};
