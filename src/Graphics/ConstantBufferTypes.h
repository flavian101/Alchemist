#pragma once
#include <DirectXMath.h>


using namespace DirectX;
struct cb_vs2DConstantBuffer
{
    
};

struct cb_vsConstantBuffer
{
	XMMATRIX WVP;
    XMMATRIX View;
    XMMATRIX Model;
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
    XMFLOAT4 color;
    XMFLOAT4 ambient;
    XMFLOAT4 diffuse;
    XMFLOAT4 specular;
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
    XMFLOAT4 baseColor;
    XMFLOAT4 emissive;
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