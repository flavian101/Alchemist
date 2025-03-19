#pragma once
#include "stdafx.h"
#include "ErrorEx.h"
#include <vector>
#include <string>
#include <codecvt>
#include <memory>

class DXGIAdapterInfo;
class DXBlender;
class SwapChain;
class DXRenderTarget;
class DXDevice;
class DXContext;
class DXDepthStencil;
class DXRasterizer;
class DXViewPort;



class Renderer {
public:
    Renderer(HWND hwnd,UINT width,UINT height);
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    ~Renderer();
   
    bool initializeImgui();
    void Bind(float red, float green, float blue, float alpha);
    void Render(UINT indexCount);
    void EndFrame();

    void setWidth(UINT width);
    void setHeight(UINT height);
    UINT GetWidth()const;
    UINT GetHeight()const;

    

private:
    bool EnumarateAdapters();
    bool createContext();
    bool createDevice();
    bool createSwapChain();
    bool createDepthStencil();
    bool CreateRenderTarget();
    bool createRasterizer();
    bool CreateViewPort();

private:
    HWND hWnd;
    UINT m_width;
    UINT m_height;
    bool enabledMsaa;
    bool isVsyncEnabled;
    std::unique_ptr<DXGIAdapterInfo> info;
    std::unique_ptr<DXDevice> device;
    std::unique_ptr<DXContext> context;
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<DXRenderTarget> renderTarget;
    std::unique_ptr<DXDepthStencil> depthStencil;
    std::unique_ptr<DXRasterizer> rasterizer;
    std::unique_ptr< DXViewPort> viewPort;

    //friend classes
    friend class SceneSerializer;
    friend class Graphics;
};
