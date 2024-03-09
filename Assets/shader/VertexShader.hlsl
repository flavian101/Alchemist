cbuffer cb_vsConstantBuffer
{
    matrix WVP; // float4x4 is the same as matrix in HLSL
    matrix Model;
};

struct VertexIn
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
};

VertexOut main(VertexIn vin)
{
    VertexOut vOut;
    vOut.posH = mul(vin.pos, WVP);
    vOut.color = vin.color;
    return vOut;
}