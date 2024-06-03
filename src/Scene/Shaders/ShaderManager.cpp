#include "ShaderManager.h"

ShaderManager::ShaderManager(Graphics& g)
	:
	m_graphics(g),
	m_vertexShader(g),
	m_pixelShader(g),
	m_layout(g)
{}

void ShaderManager::LoadShaders(std::wstring vertexShader, std::wstring pixelShader)
{
	vertexShaderPath = vertexShader;
	pixelShaderPath = pixelShader;

	std::ifstream vertexShaderFile(vertexShader);
	std::ifstream pixelShaderFile(pixelShader);

	std::stringstream vertexShaderStream, pixelShaderStream;
	vertexShaderStream << vertexShaderFile.rdbuf();
	pixelShaderStream << pixelShaderFile.rdbuf();

	m_vertexShaderCode = vertexShaderStream.str();
	m_pixelShaderCode = pixelShaderStream.str();
	
	m_vertexShader.LoadStreamVertexShader(m_vertexShaderCode);
	m_pixelShader.LoadStreamPixelShader(m_pixelShaderCode);

}

void ShaderManager::loadCompiledShader(std::wstring vertexShader, std::wstring pixelShader)
{
	this->m_vertexShader.LoadCompiledVertexShader(vertexShader);
	this->m_pixelShader.LoadCompiledPixelShader(pixelShader);

	m_vertexShaderCode = std::string(static_cast<const char*>(m_vertexShader.GetByteCode()->GetBufferPointer()),
		m_vertexShader.GetByteCode()->GetBufferSize());
	m_pixelShaderCode = std::string(static_cast<const char*>(m_pixelShader.GetByteCode()->GetBufferPointer()),
		m_pixelShader.GetByteCode()->GetBufferSize());
}

void ShaderManager::ReloadShaders()
{
	m_vertexShader.LoadStreamVertexShader(m_vertexShaderCode);
	m_pixelShader.LoadStreamPixelShader(m_pixelShaderCode);
	
}

void ShaderManager::SetShaderLayout(const std::string& layout)
{
	this->m_layout.CreateLayout(layout,m_vertexShader.GetByteCode());
}

void ShaderManager::SetVertexShaderCode(const std::string& V_code)
{
	m_vertexShaderCode = V_code;
}

void ShaderManager::SetPixelShaderCode(const std::string& P_code)
{
	m_pixelShaderCode = P_code;
}

void ShaderManager::BindShaders()
{
	this->m_vertexShader.Bind();
	this->m_layout.Bind();
	this->m_pixelShader.Bind();
	
	
}
std::wstring ShaderManager::getVertexShaderPath()
{
	return vertexShaderPath;
}
std::wstring ShaderManager::getPixelShaderPath()
{
	return pixelShaderPath;
}
std::string ShaderManager::GetVertexShaderCode()
{
	return m_vertexShaderCode;
}

std::string ShaderManager::GetPixelShaderCode()
{
	return m_pixelShaderCode;
}

void ShaderManager::ComplieAndSetShaders()
{
}
