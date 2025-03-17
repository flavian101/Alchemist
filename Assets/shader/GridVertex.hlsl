struct VS_INPUT
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

cbuffer cb_vsConstantBuffer
{
    matrix modelView : packoffset(c0);
    matrix modelViewProj : packoffset(c4);
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.Position = mul(float4(input.Position, 1.0f), modelViewProj);
    output.Color = input.Color;
    return output;
}
