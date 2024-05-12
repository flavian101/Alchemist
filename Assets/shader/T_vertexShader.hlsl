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

VertexOut main(VertexIn vin)
{
    VertexOut vs_out;
     
    vs_out.posH = mul(float4(vin.pos, 1.0f), WVP);
    vs_out.tex = vin.tex;
    
    return vs_out;
}

