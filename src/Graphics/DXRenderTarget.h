#pragma once
#include "stdafx.h"

class SwapChain;
class DXContext;
class DXDevice;
class DXDepthStencil;

class DXRenderTarget
{
public:
	DXRenderTarget(Microsoft::WRL::ComPtr<ID3D11Device4>& device, Microsoft::WRL::ComPtr<IDXGISwapChain>& swapchain, bool enableMsaa);
	~DXRenderTarget();
	bool Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context, DXDepthStencil& view);
		

	bool ClearDepthAndColor(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context, float red, float blue, float green, float alpha);
	void Release();

private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget;
};

