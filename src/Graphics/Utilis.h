#pragma once
#include "Graphics.h"
#include <vector>
#include "Vertex.h"

namespace Utils
{
	class VertexBuffer
	{
	public:
		VertexBuffer(Graphics& g);
		void InitializeVertexBuffer(std::vector<Vertex>& vertices);
		void Bind();
	private:
		Graphics& m_graphics;
		UINT stride;
		UINT offset = 0u;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;


	};
	class IndexBuffer
	{
	public:
		IndexBuffer(Graphics& g);
		void InitializeIndexBuffer(std::vector<unsigned short>& i);
		void Bind();
	private:
		Graphics& m_graphics;
		Microsoft::WRL::ComPtr< ID3D11Buffer> pIndexBuffer;
	};
	class InputLayout
	{
	public:
		InputLayout(Graphics& g);
		void CreateLayout(const std::string& keyword, Microsoft::WRL::ComPtr<ID3DBlob> pVsByteCode);
		void Bind();
	private:
		Graphics& m_graphics;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		std::vector< D3D11_INPUT_ELEMENT_DESC> layouts;
	};
	class VertexShader
	{
	public:
		VertexShader(Graphics& g);
		void LoadVertexShader(LPCWSTR path);
		ID3D10Blob* GetByteCode();
		void Bind();
	private:
		Graphics& m_graphics;
		Microsoft::WRL::ComPtr< ID3D11VertexShader> pVertexShader = nullptr;
		Microsoft::WRL::ComPtr< ID3DBlob> pShaderBlob = nullptr;

	};
	class PixelShader
	{
	public:
		PixelShader(Graphics& g);
		void LoadPixelShader(LPCWSTR path);
		void Bind();
	private:
		Graphics& m_graphics;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
		Microsoft::WRL::ComPtr< ID3DBlob> pShaderBlob = nullptr;

	};
	class Topology
	{
	public:
		Topology(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind();

	private:
		Graphics& m_graphics;
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
	class Texture
	{
	public:
		Texture(Graphics& g);
		void LoadTexture(const char* path, UINT slot = 0);
		void Bind();
		ID3D11ShaderResourceView* GetSRV() { return textureView.Get(); }
	private:
		Graphics& m_graphics;
		UINT m_slot;
		Microsoft::WRL::ComPtr< ID3D11ShaderResourceView> textureView;
		Microsoft::WRL::ComPtr < ID3D11Texture2D> pTex = nullptr;
	};
	class Sampler
	{
	public:
		Sampler(Graphics& g);

		void Bind();
	private:
		Graphics& m_graphics;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};


}

