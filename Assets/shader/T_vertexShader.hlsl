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

VertexOut main(VertexIn vin)
{
    VertexOut vs_out;
     
    vs_out.posH = mul(vin.pos, WVP);
    vs_out.tex = vin.tex;
    
    return vs_out;
}

