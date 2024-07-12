cbuffer cb_vsConstantBuffer : register(b2)
{
    matrix WVP;
    matrix View;
    matrix Model;
};

// Structures
struct Light
{
    float3 position;
    float range;
    float3 dir;
    float cone;
    float3 att;
    float intensity;
    float4 color;
    float4 ambient;
    float4 diffuse;
    float4 specular;
};

struct Material
{
    float4 baseColor;
    float4 emissive;
    float metallic;
    float roughness; // This now represents smoothness in the material properties
    float ao;
    bool hasAlbedoMap;
    bool hasNormalMap;
    bool hasMetallicMap;
    bool hasRoughnessMap;
    bool hasAOMap;
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

struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPos : POSITION;
    float3 viewPos : TEXCOORD1;
    float3 tangent : TANGENT;
};

// Textures and Sampler
Texture2D albedoMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D metallicMap : register(t2);
Texture2D smoothnessMap : register(t3); // roughnessMap
Texture2D aoMap : register(t4);
SamplerState samp : register(s0);

// Helper functions
float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159265359 * denom * denom;

    return num / max(denom, 1e-7);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / max(denom, 1e-7);
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Main pixel shader function
float4 main(VertexOut input) : SV_Target
{
    // Sample textures
    float4 albedo = material.hasAlbedoMap ? albedoMap.Sample(samp, input.tex) : float4(1.0, 1.0, 1.0, 1.0);
    float3 normalMap = material.hasNormalMap ? NormalMap.Sample(samp, input.tex).xyz : float3(0.0, 0.0, 1.0);
    float metallic = material.hasMetallicMap ? metallicMap.Sample(samp, input.tex).r : 0.0;
    float smoothness = material.hasRoughnessMap ? smoothnessMap.Sample(samp, input.tex).r : 1.0;
    float ao = material.hasAOMap ? aoMap.Sample(samp, input.tex).r : 1.0;

    // Convert smoothness to roughness
    float roughness = 1.0 - smoothness;

    // Apply material properties
    albedo *= material.baseColor;
    metallic = saturate(metallic * material.metallic);
    roughness = clamp(roughness * (1.0 - material.roughness), 0.01, 1.0); // material.roughness now represents smoothness
    ao *= material.ao;

    // Transform normal to world space
    float3 N = normalize(input.normal);
    float3 T = normalize(input.tangent);
    float3 B = normalize(cross(N, T));
    float3x3 TBN = float3x3(T, B, N);
    normalMap = normalMap * 2.0 - 1.0;
    N = normalize(mul(normalMap, TBN));

    // Calculate view and light directions
    float3 V = normalize(input.viewPos - input.worldPos);
    float3 L = normalize(-light.dir); // Assuming directional light
    float3 H = normalize(V + L);

    // Calculate necessary dot products
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);

    // Calculate Fresnel term
    float3 F0 = float3(0.04, 0.04, 0.04);
    F0 = lerp(F0, albedo.rgb, metallic);
    float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    // Calculate Normal Distribution Function
    float NDF = DistributionGGX(N, H, roughness);

    // Calculate Geometry Function
    float G = GeometrySmith(N, V, L, roughness);

    // Calculate specular BRDF
    float3 numerator = NDF * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.0001;
    float3 specular = numerator / denominator;

    // Calculate diffuse BRDF
    float3 kS = F;
    float3 kD = (1.0 - kS) * (1.0 - metallic);
    float3 diffuse = kD * albedo.rgb / 3.14159265359;

    // Combine results
    float3 ambient = light.ambient.rgb * albedo.rgb * ao;
    float3 lightColor = light.color.rgb * light.intensity;
    float3 radiance = lightColor * NdotL;

    float3 finalColor = ambient + (diffuse + specular) * radiance;

    // Apply light intensity and HDR tone mapping
    finalColor = finalColor / (finalColor + float3(1.0, 1.0, 1.0));
    
    // Apply gamma correction
    float gamma = 2.2;
    finalColor = pow(finalColor, float3(1.0 / gamma, 1.0 / gamma, 1.0 / gamma));

    return float4(finalColor, 1.0);
}
