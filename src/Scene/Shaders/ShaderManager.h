#pragma once
#include <memory>
#include <string>
#include "stdafx.h"

class Graphics;
namespace Utils
{
	class VertexShader;
	class PixelShader;
	class InputLayout;
	class Topology;
}

class ShaderManager
{
public:
	ShaderManager(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		const std::wstring& vertexShaderPath = L"", const std::wstring& pixelShaderPath = L"", bool isCompiled = false);
	~ShaderManager();

	void ReloadShaders();
	void SetShaderLayout(const std::string& layout);
	void BindShaders();


	void SetVertexShaderCode(const std::string& V_code);
	void SetPixelShaderCode(const std::string& P_code);

	// Getter methods for shaders
	Utils::VertexShader* GetVertexShader() { return m_vertexShader.get(); }
	Utils::PixelShader* GetPixelShader() { return m_pixelShader.get(); }
	Utils::InputLayout* GetLayout() { return m_layout.get(); }

	void SetTopology(Utils::Topology* topology);

	//Getters for the shader codes
	std::wstring getVertexShaderPath();
	std::wstring getPixelShaderPath();
	std::string GetVertexShaderCode();
	std::string GetPixelShaderCode();
private:
	void LoadShaders(bool isCompiled);
	void SetShaderPaths(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath);

private:
	Graphics& m_graphics;
	std::shared_ptr<Utils::VertexShader> m_vertexShader;
	std::shared_ptr<Utils::PixelShader> m_pixelShader;
	std::shared_ptr<Utils::InputLayout> m_layout;
	std::shared_ptr<Utils::Topology> m_topology;

	std::string m_vertexShaderCode;
	std::string m_pixelShaderCode;
	std::wstring m_vertexShaderPath;
	std::wstring m_pixelShaderPath;

};

