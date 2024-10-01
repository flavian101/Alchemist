#include "ShaderManager.h"
#include "Graphics/Utilis.h"
#include "Graphics/Graphics.h"
#include <sstream>
#include <fstream>
ShaderManager::ShaderManager(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type,
    const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath, bool isCompiled)
    : 
    m_topology(std::make_unique < Utils::Topology>(gfx, type)),
    m_vertexShader(std::make_unique<Utils::VertexShader>()),
    m_pixelShader(std::make_unique < Utils::PixelShader>())
{
    SetShaderPaths(vertexShaderPath, pixelShaderPath);
    LoadShaders(gfx,isCompiled);
}

ShaderManager::~ShaderManager()
{

    m_vertexShaderCode.clear();
    m_pixelShaderCode.clear();
    m_vertexShaderPath.clear();
    m_pixelShaderPath.clear();
    // Smart pointers automatically handle cleanup
}

void ShaderManager::SetShaderPaths(const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath)
{
    m_vertexShaderPath = vertexShaderPath;
    m_pixelShaderPath = pixelShaderPath;
}

void ShaderManager::LoadShaders(Graphics& gfx,bool isCompiled)
{
    if (isCompiled)
    {
        m_vertexShader->LoadCompiledVertexShader(gfx,m_vertexShaderPath);
        m_pixelShader->LoadCompiledPixelShader(gfx,m_pixelShaderPath);
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

        m_vertexShader->LoadStreamVertexShader(gfx,m_vertexShaderCode);
        m_pixelShader->LoadStreamPixelShader(gfx,m_pixelShaderCode);
    }
}

void ShaderManager::ReloadShaders(Graphics& gfx)
{
    LoadShaders(gfx,false);  // Reloads shaders from source code
}

void ShaderManager::SetShaderLayout(Graphics& gfx,const std::string& layout)
{
    m_layout = std::make_unique<Utils::InputLayout>(gfx, layout, m_vertexShader->GetByteCode());
}

void ShaderManager::BindShaders(Graphics& gfx)
{
    m_vertexShader->Bind(gfx);
    m_pixelShader->Bind(gfx);
    m_topology->Bind(gfx);
    m_layout->Bind(gfx);
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