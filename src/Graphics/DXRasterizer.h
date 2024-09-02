#pragma once
#include "stdafx.h"

class DXRasterizer
{
public:
	DXRasterizer(Microsoft::WRL::ComPtr<ID3D11Device4>& device);
	~DXRasterizer();
	void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context);

private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState1> CCWcullMode;


};

