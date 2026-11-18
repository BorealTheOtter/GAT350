#include "Lighting.hlsli"
#include "Uniforms.hlsli"

Texture2D<float4> base_1texture : register(t0, FRAGMENT_TEXTURE_SPACE);
SamplerState base_1sampler : register(s0, FRAGMENT_TEXTURE_SPACE);

Texture2D<float4> base_2texture : register(t1, FRAGMENT_TEXTURE_SPACE);
SamplerState base_2sampler : register(s1, FRAGMENT_TEXTURE_SPACE);

Texture2D<float4> base_3texture : register(t2, FRAGMENT_TEXTURE_SPACE);
SamplerState base_3sampler : register(s2, FRAGMENT_TEXTURE_SPACE);

cbuffer ObjectUniforms : register(OBJECT_SLOT, FRAGMENT_UNIFORM_SPACE)
{
    Object object;
};
cbuffer FrameUniforms : register(FRAME_SLOT, FRAGMENT_UNIFORM_SPACE)
{
    Frame frame;
};
cbuffer LightUniforms : register(LIGHT_SLOT, FRAGMENT_UNIFORM_SPACE)
{
    Lights lights;
};

cbuffer MatUniforms : register(MATERIAL_SLOT, FRAGMENT_UNIFORM_SPACE)
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
    
    float t = base_3texture.Sample(base_3sampler, input.texcoord).r;
    
    float4 albedo1 = base_1texture.Sample(base_1sampler, input.texcoord) * material.color;
    float4 albedo2 = base_2texture.Sample(base_2sampler, input.texcoord) * material.color;
    
    float4 albedo = albedo1 + albedo2;
    
    float3 diffuse = lights.ambient;
    float3 specular = 0;
    
    float3 V = normalize(frame.camPos - input.worldPos);
    for (uint i = 0; i < lights.numLights; i++)
    {
        calculateLighting(lights.light[i], material, input.worldPos, normalize(input.normal), V, diffuse, specular);
    };

    
    
    output.color = float4(albedo.rgb * diffuse + specular, albedo.a);
    return output;
}
