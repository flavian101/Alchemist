#include "Shaders.h"
#include <sstream>
#include <fstream>

Shaders::Shaders(Graphics& gfx,const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, const std::string& keyword, D3D11_PRIMITIVE_TOPOLOGY type)
	:
	m_vertexShader(gfx,vertexShaderPath),
	m_pixelShader(gfx, pixelShaderPath),
	m_layout(gfx, keyword,m_vertexShader.GetByteCode()),
	m_topology(gfx, type)
{}
Shaders::~Shaders()
{}

void Shaders::RecompileShader(Graphics& gfx)
{
	BindShader(gfx);
}

void Shaders::BindShader(Graphics& gfx)
{
	m_layout.Bind(gfx);
	m_vertexShader.Bind(gfx);
	m_topology.Bind(gfx);
	m_pixelShader.Bind(gfx);
}

void Shaders::DeleteShader()
{
	delete this;
}

void Shaders::LoadShaders(Graphics& gfx, bool isCompiled)
{
    if (isCompiled)
    {
        m_vertexShader.LoadCompiledVertexShader(gfx, m_vertexShaderPath);
        m_pixelShader.LoadCompiledPixelShader(gfx, m_pixelShaderPath);
    }
    else
    {
        std::ifstream vertexShaderFile(m_vertexShaderPath);
        std::ifstream pixelShaderFile(m_pixelShaderPath);

        if (!vertexShaderFile.is_open() || !pixelShaderFile.is_open())
        {
            throw std::runtime_error("Failed to open shader files.");
        }

        std::stringstream vertexShaderStream, pixelShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        pixelShaderStream << pixelShaderFile.rdbuf();

        m_vertexShaderCode = vertexShaderStream.str();
        m_pixelShaderCode = pixelShaderStream.str();

        m_vertexShader.LoadStreamVertexShader(gfx, m_vertexShaderCode);
        m_pixelShader.LoadStreamPixelShader(gfx, m_pixelShaderCode);
    }
}

void Shaders::SetShaderPaths(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath)
{
    m_vertexShaderPath = vertexShaderPath;
    m_pixelShaderPath = pixelShaderPath;

}

void Shaders::SetVertexShaderCode(const std::string& V_code)
{
    m_vertexShaderCode = V_code;

}

void Shaders::SetPixelShaderCode(const std::string& P_code)
{
    m_pixelShaderCode = P_code;
}

std::wstring Shaders::getVertexShaderPath()
{
    return m_vertexShaderPath;
}

std::wstring Shaders::getPixelShaderPath()
{
    return m_pixelShaderPath;
}

std::string Shaders::GetVertexShaderCode()
{
    return m_vertexShaderCode;
}

std::string Shaders::GetPixelShaderCode()
{
    return m_pixelShaderCode;
}
