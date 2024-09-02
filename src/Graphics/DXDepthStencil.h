#pragma once
#include "stdafx.h"
class DXDevice;


class DXDepthStencil
{
public:
	DXDepthStencil(DXDevice& device, UINT width, UINT height, bool enableMsaa);
	~DXDepthStencil();
	void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context);
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& getDSV();
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};

