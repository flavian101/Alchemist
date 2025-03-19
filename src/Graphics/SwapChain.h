#pragma once
#include "stdafx.h"
#include <memory>

class DXGIAdapterInfo;
class DXDevice;


class SwapChain
{
public:
	SwapChain(HWND hwnd, DXGIAdapterInfo& info, DXDevice& device,UINT width,UINT height, bool enableMsaa);
	~SwapChain();
	void Resize(UINT width, UINT height);
	void Present(bool isVsyncEnabled);
	Microsoft::WRL::ComPtr<IDXGISwapChain>& getIDXGISwapChain();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>pSwapChain;

	
};

