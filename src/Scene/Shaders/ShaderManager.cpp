#include "ShaderManager.h"
#include "Graphics/Utilis.h"
#include "Graphics/Graphics.h"
#include <sstream>
#include <fstream>
ShaderManager::ShaderManager(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY type,
    const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, bool isCompiled)
    : 
    m_graphics(g),
    m_topology(std::make_unique < Utils::Topology>(g, type)),
    m_vertexShader(std::make_unique<Utils::VertexShader>(g)),
    m_pixelShader(std::make_unique < Utils::PixelShader>(g))
{
    SetShaderPaths(vertexShaderPath, pixelShaderPath);
    LoadShaders(isCompiled);
}

ShaderManager::~ShaderManager()
{
    // Smart pointers automatically handle cleanup
}

void ShaderManager::SetShaderPaths(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath)
{
    m_vertexShaderPath = vertexShaderPath;
    m_pixelShaderPath = pixelShaderPath;
}

void ShaderManager::LoadShaders(bool isCompiled)
{
    if (isCompiled)
    {
        m_vertexShader->LoadCompiledVertexShader(m_vertexShaderPath);
        m_pixelShader->LoadCompiledPixelShader(m_pixelShaderPath);
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

        m_vertexShader->LoadStreamVertexShader(m_vertexShaderCode);
        m_pixelShader->LoadStreamPixelShader(m_pixelShaderCode);
    }
}

void ShaderManager::ReloadShaders()
{
    LoadShaders(false);  // Reloads shaders from source code
}

void ShaderManager::SetShaderLayout(const std::string& layout)
{
    m_layout = std::make_unique<Utils::InputLayout>(m_graphics, layout, m_vertexShader->GetByteCode());
}

void ShaderManager::BindShaders()
{
    m_vertexShader->Bind();
    m_pixelShader->Bind();
    m_topology->Bind();
    m_layout->Bind();
}
void ShaderManager::SetVertexShaderCode(const std::string& V_code)
{
    m_vertexShaderCode = V_code;
}

void ShaderManager::SetPixelShaderCode(const std::string& P_code)
{
    m_pixelShaderCode = P_code;
}
std::wstring ShaderManager::getVertexShaderPath()
{
    return m_vertexShaderPath;
}
std::wstring ShaderManager::getPixelShaderPath()
{
    return m_pixelShaderPath;
}
std::string ShaderManager::GetVertexShaderCode()
{
    return m_vertexShaderCode;
}

std::string ShaderManager::GetPixelShaderCode()
{
    return m_pixelShaderCode;
}