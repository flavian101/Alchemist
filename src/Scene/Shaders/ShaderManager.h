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
	ShaderManager(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	~ShaderManager();
	void LoadShaders(const std::wstring& vertexShader,const  std::wstring& pixelShader);
	void loadCompiledShader(std::wstring vertexShader, std::wstring pixelShader);
	void ReloadShaders();
	void SetShaderLayout(const std::string& layout);
	void SetVertexShaderCode(const std::string& V_code);
	void SetPixelShaderCode(const std::string& P_code);
	void BindShaders();
	// Getter methods for shaders
	Utils::VertexShader* GetVertexShader() { return m_vertexShader.get(); }
	Utils::PixelShader* GetPixelShader() { return m_pixelShader.get(); }
	Utils::InputLayout* GetLayout() { return m_layout.get(); }
	void SetTopology(Utils::Topology* topology);
	std::wstring getVertexShaderPath();
	std::wstring getPixelShaderPath();
	//Getters for the shader codes
	std::string GetVertexShaderCode();
	std::string GetPixelShaderCode();
private:
	void ComplieAndSetShaders();
private:
	Graphics& m_graphics;
	std::shared_ptr<Utils::VertexShader> m_vertexShader;
	std::shared_ptr<Utils::PixelShader> m_pixelShader;
	std::shared_ptr<Utils::InputLayout> m_layout;
	std::string m_vertexShaderCode;
	std::string m_pixelShaderCode;
	std::shared_ptr<Utils::Topology> m_topology;

	std::wstring vertexShaderPath;
	std::wstring pixelShaderPath;

};

