#pragma once
#include "stdafx.h"

class DXViewPort
{
public:
	DXViewPort(UINT Width,UINT height);
	~DXViewPort();
	bool Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context);

private:
	D3D11_VIEWPORT viewport;
};

