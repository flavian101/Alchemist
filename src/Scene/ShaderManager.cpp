#include "ShaderManager.h"

ShaderManager::ShaderManager(Graphics& g)
	:
	m_graphics(g),
	m_vertexShader(Utils::VertexShader(g)),
	m_pixelShader(Utils::PixelShader(g)),
	m_layout(g)
{}

void ShaderManager::LoadShaders(LPCWSTR vertexShader, LPCWSTR pixelShader)
{
	this->m_vertexShader.LoadVertexShader(vertexShader);
	this->m_layout.CreateLayout(m_vertexShader.GetByteCode());
	this->m_pixelShader.LoadPixelShader(pixelShader);
}

void ShaderManager::BindShaders()
{
	this->m_vertexShader.Bind();
	this->m_layout.Bind();
	this->m_pixelShader.Bind();
	
}
