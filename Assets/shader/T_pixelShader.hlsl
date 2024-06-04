struct Light
{
    float3 pointPos;
    float3 spotPos;
    float range;
    float3 dir;
    float cone;
    float3 att;
    float4 ambient;
    float4 diffuse;
};
cbuffer cb_psConstantBuffer 
{
    Light light;
}; 

cbuffer cb_vsConstantBuffer 
{
    matrix WVP : packoffset(c0);
    matrix Model : packoffset(c4);
};



struct VertexIn
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
    float3 normal : NORMAL;

};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
    float3 normal : NORMAL;

};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(VertexOut input) :SV_Target
{
    input.normal = normalize(input.normal);
    
    float4 diffuse = tex.Sample(samp, input.tex);
    float3 finalColor;
    
    finalColor = diffuse * light.ambient;
    finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);
	
    return float4(finalColor, diffuse.a);
}