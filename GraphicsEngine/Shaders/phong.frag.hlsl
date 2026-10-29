#include "Lighting.hlsli"

Texture2D<float4> u_texture : register(t0, space2);
SamplerState u_sampler : register(s0, space2);

cbuffer VertexUniforms : register(b0, space3)
{
    float4x4 mvp;
    float4x4 model;
    float3 camPos;
    float padding1;
};
cbuffer LightUniforms : register(b1, space3)
{
    Light light[8];
    float3 ambient;
    uint numLights;
};
cbuffer MatUniforms : register(b2, space3)
{
    Material material;
};

struct Input
{
    float2 texcoord : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
    float3 normal : TEXCOORD2;
};

struct Output
{
    float4 color : SV_Target0;
};

Output main(Input input)
{
    Output output;
    
    float4 albedo = u_texture.Sample(u_sampler, input.texcoord) * material.color;
    float3 diffuse = ambient;
    float3 specular = 0;
    
    float3 V = normalize(camPos - input.worldPos);
    for (uint i = 0; i < numLights; i++)
    {
        calculateLighting(light[i], material, input.worldPos, normalize(input.normal), V, diffuse, specular);
    };

    
    
    output.color = float4(albedo.rgb * diffuse + specular, albedo.a);
    return output;
}
