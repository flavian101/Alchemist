#pragma once
#include "stdafx.h"
#include <vector>
#include <iostream>


class DXGIAdapterInfo
{
public:
	DXGIAdapterInfo();
	~DXGIAdapterInfo();
	void PrintAdapterInfo();
	void SetHighestResolution(Microsoft::WRL::ComPtr<ID3D11Device4>& device,
		Microsoft::WRL::ComPtr<IDXGISwapChain>& swapChain,HWND hwnd);
	UINT getHeight();
	UINT getWidth();
private:
	void choosePrimaryAdapter();
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> EnumerateAdapters();
	DXGI_MODE_DESC GetHighestResolution(Microsoft::WRL::ComPtr <IDXGIAdapter1>& adapter, Microsoft::WRL::ComPtr<IDXGIOutput1>& output);
	bool isPrimaryAdapter(Microsoft::WRL::ComPtr<IDXGIOutput> output);


private:
	Microsoft::WRL::ComPtr<IDXGIFactory2> pFactory;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> pAdapter;
	std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> m_adapters;
	Microsoft::WRL::ComPtr<IDXGIDevice1> pDDevice;
	DXGI_MODE_DESC m_bestmode;

private:
	friend class SwapChain;
	friend class DXDevice;

};

