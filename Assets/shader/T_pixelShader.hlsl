cbuffer cb_vsConstantBuffer
{
    matrix modelView : packoffset(c0);
    matrix modelViewProj : packoffset(c4);
};

// Light and Material Structures
struct Light
{
    float3 position;
    float range;
    float3 dir;
    float cone;
    float3 att;
    float intensity;
    float4 color, ambient, diffuse, specular;
};

struct Material
{
    float4 baseColor, emissive, specular;
    bool hasAlbedoMap, hasNormalMap, hasMetallicMap, hasRoughnessMap, hasAOMap, hasSpecular;
    float metallic, roughness, ao;
};

// Constant Buffers
cbuffer cb_psConstantBuffer : register(b0)
{
    Light light;
};
cbuffer cb_psMaterialBuffer : register(b1)
{
    Material material;
};

// Vertex Output
struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL, worldPos : POSITION1, viewPos : TEXCOORD1, tangent : TANGENT;
};

// Textures and Sampler
Texture2D albedoMap : register(t0), NormalMap : register(t1), metallicMap : register(t2), roughnessMap : register(t3), aoMap : register(t4), specularMap : register(t5);
SamplerState samp : register(s0);

// Fresnel-Schlick Approximation
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// GGX Distribution Function
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a2 = roughness * roughness * roughness * roughness;
    float NdotH2 = max(dot(N, H), 0.0) * max(dot(N, H), 0.0);
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (3.14159265 * denom * denom);
}

// Schlick-GGX Geometry Function
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

// Smith Geometry Function
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    return GeometrySchlickGGX(max(dot(N, V), 0.0), roughness) * GeometrySchlickGGX(max(dot(N, L), 0.0), roughness);
}

// Main Pixel Shader
float4 main(VertexOut input) : SV_Target
{
    float4 albedo = material.hasAlbedoMap ? albedoMap.Sample(samp, input.tex) : material.baseColor;
    float3 N = normalize(mul((material.hasNormalMap ? NormalMap.Sample(samp, input.tex).xyz * 2.0 - 1.0 : input.normal), float3x3(normalize(input.tangent), normalize(cross(input.normal, input.tangent)), normalize(input.normal))));
    float metallic = material.hasMetallicMap ? metallicMap.Sample(samp, input.tex).r : material.metallic;
    float roughness = material.hasRoughnessMap ? 1.0 - roughnessMap.Sample(samp, input.tex).r : material.roughness;
    float ao = material.hasAOMap ? aoMap.Sample(samp, input.tex).r : material.ao;
    float3 specularColor = material.hasSpecular ? specularMap.Sample(samp, input.tex).rgb : material.specular.rgb;
    
    float3 V = normalize(input.viewPos - input.worldPos);
    float3 L = normalize(-light.dir);
    float3 H = normalize(V + L);
    float NdotV = max(dot(N, V), 0.0), NdotL = max(dot(N, L), 0.0);
    
    float3 F0 = lerp(specularColor, albedo.rgb, metallic);
    float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    
    float3 specularBRDF = (DistributionGGX(N, H, roughness) * GeometrySmith(N, V, L, roughness) * F) / max(4.0 * NdotV * NdotL, 1e-4);
    float3 diffuse = (1.0 - F) * (1.0 - metallic) * albedo.rgb / 3.14159265;
    
    float3 radiance = light.color.rgb * light.intensity * NdotL;
    float3 finalColor = light.ambient.rgb * albedo.rgb * ao + (diffuse + specularBRDF) * radiance;
    finalColor = pow(finalColor / (finalColor + 1.0), 1.0 / 2.2);
    
    return float4(finalColor, 1.0);
}