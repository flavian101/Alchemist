#pragma once
#include "stdafx.h"

class DXGIAdapterInfo;

class DXDevice
{
public:
	DXDevice(DXGIAdapterInfo& info);
	~DXDevice();
	Microsoft::WRL::ComPtr<ID3D11Device4>& getDevice();
	UINT getMsaaLevel();

private:
	void CheckMsaaLevel();
private:
	Microsoft::WRL::ComPtr<ID3D11Device4> pDevice;
	//Microsoft::WRL::ComPtr< ID3D11DeviceContext4> pImmediateContext;
	UINT m4xMsaaQuality;
private:
	friend class SwapChain;
};

