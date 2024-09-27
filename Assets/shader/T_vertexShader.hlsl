cbuffer cb_vsConstantBuffer
{
    matrix modelView : packoffset(c0);
    matrix modelViewProj : packoffset(c4);
};

struct VertexIn
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPos : POSITION1;
    float3 viewPos : TEXCOORD1;
    float3 tangent : TANGENT;
};

VertexOut main(VertexIn input)
{
    VertexOut output;

    // Transform position to world space
    float4 worldPos = mul(float4(input.pos, 1.0f), modelView);
    output.worldPos = worldPos.xyz;

    // Transform position to homogeneous clip space
    output.posH = mul(float4(input.pos, 1.0f), modelViewProj);

    // Pass through texture coordinates
    output.tex = input.tex;

    // Transform normal to world space
    output.normal = normalize(mul(float4(input.normal, 0.0f), modelView).xyz);

    // Transform tangent to world space
    output.tangent = normalize(mul(float4(input.tangent, 0.0f), modelView).xyz);

    // Calculate view position
    float4 viewPos = mul(worldPos, modelView);
    output.viewPos = viewPos.xyz;

    return output;
}
