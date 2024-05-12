cbuffer cb_vsConstantBuffer : register(b0)
{
    matrix WVP : packoffset(c0);
    matrix Model : packoffset(c4);
};

struct VertexIn
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(VertexOut ps_in) :SV_Target
{
	return tex.Sample(samp, ps_in.tex);
}