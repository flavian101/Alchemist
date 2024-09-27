#pragma once
#include <DirectXMath.h>


using namespace DirectX;
struct cb_vs2DConstantBuffer
{
    
};

struct cb_vsConstantBuffer
{
    DirectX::XMMATRIX modelView;
    DirectX::XMMATRIX modelViewProj;
};
struct LightStruct
{
	LightStruct()
	{
		ZeroMemory(this, sizeof(LightStruct));
	}
    XMFLOAT3 position;
    float range;
    XMFLOAT3 dir;
    float cone;
    XMFLOAT3 att;
    float intensity;
     alignas(16) XMFLOAT4 color;
     alignas(16) XMFLOAT4 ambient;
     alignas(16)XMFLOAT4 diffuse;
     alignas(16)XMFLOAT4 specular;
};
struct cb_psConstantBuffer
{
	LightStruct lightStruct;
};

struct MaterialStruct
{
    MaterialStruct()
    {
        ZeroMemory(this, sizeof(MaterialStruct));
    }
    alignas(16)XMFLOAT4 baseColor;
    alignas(16)XMFLOAT4 emissive;
    BOOL hasAlbedoMap;
    BOOL hasNormalMap;
    BOOL hasMetallicMap;
    BOOL hasRoughnessMap;
    BOOL hasAOMap;
    float metallic;
    float roughness;
    float ao;
    float pad1[4];
   
};

struct cb_psMaterialBuffer
{
    MaterialStruct materialStruct;
};