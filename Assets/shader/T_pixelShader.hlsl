cbuffer cb_vsConstantBuffer
{
	matrix WVP;
	matrix Model;
};

struct VertexIn
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D tex;
SamplerState samp;
float4 main(VertexOut ps_in) :SV_Target
{
	return tex.Sample(samp, ps_in.tex);
}