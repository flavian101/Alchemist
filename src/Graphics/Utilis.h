#pragma once
#include <vector>
#include <codecvt>
#include <stdafx.h>

class Graphics;
class Vertex;

namespace Utils
{
	class VertexBuffer
	{
	public:
		VertexBuffer(Graphics& g,const std::vector<Vertex>& vertices);
		~VertexBuffer();
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
		IndexBuffer(Graphics& g,const std::vector<unsigned short>& indices);
		~IndexBuffer();
		void Bind();
	private:
		Graphics& m_graphics;
		Microsoft::WRL::ComPtr< ID3D11Buffer> pIndexBuffer;
	};
	class InputLayout
	{
	public:
		InputLayout(Graphics& g, const std::string& keyword, Microsoft::WRL::ComPtr<ID3DBlob> pVsByteCode);
		~InputLayout();
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
		~VertexShader();
		void LoadStreamVertexShader(std::string stream);
		void LoadCompiledVertexShader(std::wstring path);
		ID3DBlob* GetByteCode();
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
		~PixelShader();
		void LoadStreamPixelShader(std::string stream);
		void LoadCompiledPixelShader(std::wstring path);
		ID3DBlob* GetByteCode();
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
		Texture(Graphics& g, const char* path);
		~Texture();
		void Bind(UINT slot= 0);
		ID3D11ShaderResourceView* GetSRV() { return textureView.Get(); }
	private:
		Graphics& m_graphics;
		UINT m_slot = 0u;
		Microsoft::WRL::ComPtr< ID3D11ShaderResourceView> textureView;
		Microsoft::WRL::ComPtr < ID3D11Texture2D> pTex = nullptr;
	};
	class Sampler
	{
	public:
		Sampler(Graphics& g);
		~Sampler();
		void Bind();
	private:
		Graphics& m_graphics;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};


}

