#pragma once
#include <memory>
#include <string>
#include "stdafx.h"
#include "Graphics/Utilis.h"

class Shaders
{
public:
	Shaders(Graphics& gfx,
		const std::wstring& vertexShaderPath,
		const std::wstring& pixelShaderPath,
		const std::string& keyword,
		D3D11_PRIMITIVE_TOPOLOGY type);
	~Shaders();

	void RecompileShader(Graphics& gfx);
	void BindShader(Graphics& gfx);
	void DeleteShader();

	void SetVertexShaderCode(const std::string& V_code);
	void SetPixelShaderCode(const std::string& P_code);

	//Getters for the shader codes
	std::wstring getVertexShaderPath();
	std::wstring getPixelShaderPath();
	std::string GetVertexShaderCode();
	std::string GetPixelShaderCode();
private:
	void LoadShaders(Graphics& gfx, bool isCompiled);
	void SetShaderPaths(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath);

private:
	Utils::VertexShader m_vertexShader;
	Utils::PixelShader m_pixelShader;
	Utils::InputLayout m_layout;
	Utils::Topology m_topology;
	
	std::string m_vertexShaderCode;
	std::string m_pixelShaderCode;
	std::wstring m_vertexShaderPath;
	std::wstring m_pixelShaderPath;
};

