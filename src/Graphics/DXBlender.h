#pragma once
#include "stdafx.h"

class DXBlender
{
public:
	DXBlender(Microsoft::WRL::ComPtr<ID3D11Device4>& device);
	~DXBlender();
	void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext4>& context);
	void Release();
private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

};

