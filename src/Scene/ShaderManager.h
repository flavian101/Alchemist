#pragma once
#include "Graphics\Utilis.h"
#include <memory>


class ShaderManager
{
public:
	ShaderManager(Graphics& g);

	void LoadShaders(LPCWSTR vertexShader, LPCWSTR pixelShader);
	void SetShaderLayout(const D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements);
	void BindShaders();
	// Getter methods for shaders
	Utils::VertexShader GetVertexShader() { return m_vertexShader; }
	Utils::PixelShader GetPixelShader() { return m_pixelShader; }
	Utils::InputLayout GetLayout() { return m_layout; }
private:
	Graphics& m_graphics;
	Utils::VertexShader m_vertexShader; 
	Utils::PixelShader m_pixelShader;   
	Utils::InputLayout m_layout;

};

