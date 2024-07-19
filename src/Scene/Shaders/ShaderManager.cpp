#include "ShaderManager.h"
#include "Graphics/Utilis.h"
#include "Graphics/Graphics.h"
#include <sstream>
#include <fstream>

ShaderManager::ShaderManager(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY type)
	:
	m_graphics(g),
	m_topology(std::make_unique < Utils::Topology>(g,type)),
	m_vertexShader(std::make_unique<Utils::VertexShader>(g)),
	m_pixelShader(std::make_unique < Utils::PixelShader>(g)),
	m_layout(std::make_unique < Utils::InputLayout>(g))
{}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::LoadShaders(const std::wstring& vertexShader,const std::wstring& pixelShader)
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
	vertexShaderStream.clear();
	pixelShaderStream.clear();
	m_vertexShader->LoadStreamVertexShader(m_vertexShaderCode);
	m_pixelShader->LoadStreamPixelShader(m_pixelShaderCode);

}

void ShaderManager::loadCompiledShader(std::wstring vertexShader, std::wstring pixelShader)
{
	this->m_vertexShader->LoadCompiledVertexShader(vertexShader);
	this->m_pixelShader->LoadCompiledPixelShader(pixelShader);

	m_vertexShaderCode = std::string(static_cast<const char*>(m_vertexShader->GetByteCode()->GetBufferPointer()),
		m_vertexShader->GetByteCode()->GetBufferSize());
	m_pixelShaderCode = std::string(static_cast<const char*>(m_pixelShader->GetByteCode()->GetBufferPointer()),
		m_pixelShader->GetByteCode()->GetBufferSize());
}

void ShaderManager::ReloadShaders()
{
	m_vertexShader->LoadStreamVertexShader(m_vertexShaderCode);
	m_pixelShader->LoadStreamPixelShader(m_pixelShaderCode);
	
}

void ShaderManager::SetShaderLayout(const std::string& layout)
{
	m_layout->CreateLayout(layout,m_vertexShader->GetByteCode());
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
	m_topology->Bind();
	m_vertexShader->Bind();
	m_layout->Bind();
	m_pixelShader->Bind();
	
	
}
void ShaderManager::SetTopology(Utils::Topology* topology)
{
	m_topology.reset(topology);
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
