#include "ShaderManager.h"
#include "Graphics/Utilis.h"
#include "Graphics/Graphics.h"
#include "Scene/Shaders/Shaders.h"

ShaderManager::ShaderManager(Graphics& gfx)
{
	shaders["Grid"] = std::make_unique<Shaders>(gfx,
		L"Assets/shader/GridVertex.cso",
		L"Assets/shader/GridPixel.cso",
		"POSITION|COLOR",
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	shaders["Textured"] = std::make_unique<Shaders>(gfx,
		L"Assets/shader/T_vertexShader.cso",
		L"Assets/shader/T_pixelShader.cso",
		"POSITION|TEXCOORD|NORMAL|TANGENT",
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

ShaderManager::~ShaderManager()
{

  
}

void ShaderManager::BindShader(Graphics& gfx,const char* shaderName)
{
	auto it = shaders.find(shaderName);
	if (it != shaders.end())
	{
		it->second->BindShader(gfx);
	}
}

void ShaderManager::UnBindShader(Graphics& gfx,const char* shaderName)
{
	auto it = shaders.find(shaderName);
	if (it != shaders.end())
	{
		it->second->DestroyShader(gfx);
	}
}






