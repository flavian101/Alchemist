#pragma once
#include "stdafx.h"
#include "ErrorEx.h"
#include <memory>

class Graphics
{
public:
Graphics(HWND hwnd, int Width, int Height, bool FullScreen);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void ClearDepthColor(float red, float green, float blue);
	void Render(UINT IndexCount);
	void End();
	void Resize(UINT width, UINT height);
	void EnableImgui();
	void DisableImgui();
	bool isImguiEnabled()const noexcept;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext();
	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	void controlWindow();
	void SetProjectionMatrix(DirectX::FXMMATRIX projectionMatrix);
	DirectX::XMMATRIX GetProjectionMatrix() const;
	void SetViewMatrix(DirectX::FXMMATRIX viewMatrix);
	DirectX::XMMATRIX GetViewMatrix()const;


private:
	bool Intitalize(); 
	bool CreateDevice();
	bool DepthStencil();
	void ToggleMsaa(bool enableMsaa);

	

	//
private:
	HWND hWnd;
	Microsoft::WRL::ComPtr<IDXGIDevice1> device;
	Microsoft::WRL::ComPtr <IDXGIFactory1> factory;
	Microsoft::WRL::ComPtr <IDXGIAdapter1> adapter;
	Microsoft::WRL::ComPtr <IDXGIOutput> adapterOutput;
	unsigned int numModes, i, numerator{}, denominator{};
	int m_width, m_height;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget = nullptr;
	D3D11_VIEWPORT vp;
	bool isFullscreenEnabled = TRUE;
	bool isVsyncEnabled = false;
	bool enableMsaa = false;
	UINT m4xMsaaQuality;
	//depth
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DSLessEqual;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	//imgui
	bool imguiEnabled = true;
	//matrices
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX view;

	//raster states
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> CCWcullMode;



};

