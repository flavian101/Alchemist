#pragma once
#include "stdafx.h"
class SwapChain;
class DXDevice;


class DXContext
{
public:
	DXContext(DXDevice& device);
	~DXContext();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& getContext();
private:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext4> pContext;
private:
	friend class DXDevice;
};

