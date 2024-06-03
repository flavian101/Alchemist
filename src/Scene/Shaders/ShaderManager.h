#pragma once
#include "Graphics\Utilis.h"
#include <memory>



class ShaderManager
{
public:
	ShaderManager(Graphics& g);

	void LoadShaders(std::wstring vertexShader, std::wstring pixelShader);
	void loadCompiledShader(std::wstring vertexShader, std::wstring pixelShader);
	void ReloadShaders();
	void SetShaderLayout(const std::string& layout);
	void SetVertexShaderCode(const std::string& V_code);
	void SetPixelShaderCode(const std::string& P_code);
	void BindShaders();
	// Getter methods for shaders
	Utils::VertexShader GetVertexShader() { return m_vertexShader; }
	Utils::PixelShader GetPixelShader() { return m_pixelShader; }
	Utils::InputLayout GetLayout() { return m_layout; }
	std::wstring getVertexShaderPath();
	std::wstring getPixelShaderPath();
	//Getters for the shader codes
	std::string GetVertexShaderCode();
	std::string GetPixelShaderCode();
private:
	void ComplieAndSetShaders();
private:
	Graphics& m_graphics;
	Utils::VertexShader m_vertexShader;
	Utils::PixelShader m_pixelShader;
	Utils::InputLayout m_layout;
	std::string m_vertexShaderCode;
	std::string m_pixelShaderCode;

	std::wstring vertexShaderPath;
	std::wstring pixelShaderPath;

};

