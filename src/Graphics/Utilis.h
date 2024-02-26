#pragma once
#include "Graphics.h"
#include <vector>
#include "Vertex.h"

namespace Utils
{
	class VertexBuffer
	{
	public:
		VertexBuffer(Graphics& g, std::vector<Vertex>& vertices);
		void Bind(Graphics& g);
	private:
		UINT stride;
		UINT offset = 0u;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;


	};
	class IndexBuffer
	{
	public:
		IndexBuffer(Graphics& g, std::vector<unsigned short>& i);
		void Bind(Graphics& g);
	private:
		std::vector<unsigned short>& indices ;
		Microsoft::WRL::ComPtr< ID3D11Buffer> pIndexBuffer;
	};
	class InputLayout
	{
	public:
		InputLayout(Graphics& g, Microsoft::WRL::ComPtr<ID3DBlob> pVsByteCode);
		void Bind(Graphics& g);
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	};
	class VertexShader
	{
	public:
		VertexShader(Graphics& g, LPCWSTR path);
		ID3D10Blob* GetByteCode();
		void Bind(Graphics& g);
	private:
		Microsoft::WRL::ComPtr< ID3D11VertexShader> pVertexShader = nullptr;
		Microsoft::WRL::ComPtr< ID3DBlob> pShaderBlob = nullptr;

	};
	class PixelShader
	{
	public:
		PixelShader(Graphics& g, LPCWSTR path);

		void Bind(Graphics& g);
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
		Microsoft::WRL::ComPtr< ID3DBlob> pShaderBlob = nullptr;

	};
	class Topology
	{
	public:
		Topology(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(Graphics& g);

	private:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
	class Texture
	{
	public:
		Texture(Graphics& g, const char* path, UINT slot = 0);
		void Bind(Graphics& g);
	private:
		UINT m_slot;
		Microsoft::WRL::ComPtr< ID3D11ShaderResourceView> textureView;
		Microsoft::WRL::ComPtr < ID3D11Texture2D> pTex = nullptr;
	};
	class Sampler
	{
	public:
		Sampler(Graphics& g);

		void Bind(Graphics& g);
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};


}

