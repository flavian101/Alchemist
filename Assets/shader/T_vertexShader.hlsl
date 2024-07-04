cbuffer cb_vsConstantBuffer 
{
    matrix WVP : packoffset(c0);
    matrix View : packoffset(c4);
    matrix Model : packoffset(c8);
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
    float2 tex : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPos : POSITION;
    float3 viewPos : TEXCOORD1;
    float3 tangent : TANGENT;


};


VertexOut main(VertexIn input)
{
    VertexOut output;

   // Transform position to world space
    float4 worldPos = mul(float4(input.pos, 1.0f), Model);
    output.worldPos = worldPos.xyz;

    // Transform position to homogeneous clip space
    output.posH = mul(worldPos, WVP);

    // Pass through texture coordinates
    output.tex = input.tex;

    // Transform normal to world space
    output.normal = normalize(mul(float4(input.normal, 0.0f), Model).xyz);

    // Transform tangent to world space
    output.tangent = normalize(mul(float4(input.tangent, 0.0f), Model).xyz);

    // Calculate view position
    float4 viewPos = mul(worldPos, View);
    output.viewPos = viewPos.xyz;

    return output;
}

