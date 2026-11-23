#include "Lighting.hlsli"
#include "Uniforms.hlsli"

Texture2D<float4> base_texture : register(t0, FRAGMENT_TEXTURE_SPACE);
SamplerState base_sampler : register(s0, FRAGMENT_TEXTURE_SPACE);

Texture2D<float4> specular_texture : register(t1, FRAGMENT_TEXTURE_SPACE);
SamplerState specular_sampler : register(s1, FRAGMENT_TEXTURE_SPACE);

Texture2D<float4> emissive_texture : register(t2, FRAGMENT_TEXTURE_SPACE);
SamplerState emissive_sampler : register(s2, FRAGMENT_TEXTURE_SPACE);

Texture2D<float4> normal_texture : register(t3, FRAGMENT_TEXTURE_SPACE);
SamplerState normal_sampler : register(s3, FRAGMENT_TEXTURE_SPACE);

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
    
    float specularMask = specular_texture.Sample(specular_sampler, input.texcoord).r;
    
    float3 emissive = emissive_texture.Sample(emissive_sampler, input.texcoord).rgb * material.emissiveColor;
    
    float4 albedo = base_texture.Sample(base_sampler, input.texcoord) * material.color;
    float3 diffuse = lights.ambient;
    float3 specular = 0;
    
    float3 V = normalize(frame.camPos - input.worldPos);
    for (uint i = 0; i < lights.numLights; i++)
    {
        calculateLighting(lights.light[i], material, input.worldPos, normalize(input.normal), V, diffuse, specular);
    };

    
    
    output.color = float4(albedo.rgb * diffuse + specular * material.specularColor * specularMask + emissive, albedo.a);
    return output;
}
