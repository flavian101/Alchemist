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
		VertexBuffer(Graphics& gfx,const std::vector<Vertex>& vertices);
		~VertexBuffer();
		void Bind(Graphics& gfx);
		void Unbind(Graphics& gfx);
	private:
		UINT stride;
		UINT offset = 0u;
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;


	};
	class IndexBuffer
	{
	public:
		IndexBuffer(Graphics& gfx,const std::vector<unsigned short>& indices);
		~IndexBuffer();
		void Bind(Graphics& gfx);
		void UnBind(Graphics& gfx);
	private:
		Microsoft::WRL::ComPtr< ID3D11Buffer> pIndexBuffer;
	};
	class InputLayout
	{
	public:
		InputLayout(Graphics& gfx, const std::string& keyword, Microsoft::WRL::ComPtr<ID3DBlob> pVsByteCode);
		~InputLayout();
		void Bind(Graphics& gfx);
		void UnBind(Graphics& gfx);
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		std::vector< D3D11_INPUT_ELEMENT_DESC> layouts;
	};
	class VertexShader
	{
	public:
		VertexShader(Graphics& gfx, const std::wstring& path);
		~VertexShader();
		void LoadStreamVertexShader(Graphics& gfx,std::string stream);
		void LoadCompiledVertexShader(Graphics& gfx,std::wstring path);
		ID3DBlob* GetByteCode();
		void Bind(Graphics& gfx);
		void UnBind(Graphics& gfx);
	private:
		Microsoft::WRL::ComPtr< ID3D11VertexShader> pVertexShader = nullptr;
		Microsoft::WRL::ComPtr< ID3DBlob> pShaderBlob = nullptr;

	};
	class PixelShader
	{
	public:
		PixelShader(Graphics& gfx, const std::wstring path);
		~PixelShader();
		void LoadStreamPixelShader(Graphics& gfx,std::string stream);
		void LoadCompiledPixelShader(Graphics& gfx,std::wstring path);
		ID3DBlob* GetByteCode();	
		void Bind(Graphics& gfx);
		void Unbind(Graphics& gfx);
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader = nullptr;
		Microsoft::WRL::ComPtr< ID3DBlob> pShaderBlob = nullptr;

	};
	class Topology
	{
	public:
		Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(Graphics& gfx);

	private:
		D3D11_PRIMITIVE_TOPOLOGY type;
	};
	class Texture
	{
	public:
		Texture(Graphics& gfx, const char* path);
		~Texture();
		void Bind(Graphics& gfx,UINT slot= 0);
		ID3D11ShaderResourceView* GetSRV() { return textureView.Get(); }
		void UnBind(Graphics& gfx);
	private:
		UINT m_slot = 0u;
		Microsoft::WRL::ComPtr< ID3D11ShaderResourceView> textureView;
		Microsoft::WRL::ComPtr < ID3D11Texture2D> pTex = nullptr;
	};
	class Sampler
	{
	public:
		Sampler(Graphics& gfx);
		~Sampler();
		void Bind(Graphics& gfx);
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	};


}

