#include "Lighting.hlsli"

cbuffer Uniforms : register(b0, space1)
{
    float4x4 mvp;
    float4x4 model;
};
cbuffer LightUniforms : register(b1, space1)
{
    Light light;
};
cbuffer MatUniforms : register(b2, space1)
{
    Material material;
};

struct Input
{
    float3 position : TEXCOORD0;
    float2 texcoord : TEXCOORD1;
    float3 normal : TEXCOORD2;
};

struct Output
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
    float3 diffuse : TEXCOORD1;
    float3 specular : TEXCOORD2;
};

Output main(Input input)
{
    Output output;
    output.texcoord = (input.texcoord * material.tiling) + material.offset;
    output.position = mul(mvp, float4(input.position, 1.0f));
    
    float3 worldPos = mul(model, float4(input.position, 1.0f)).xyz;
    
    //DIFFUSE
    float3 N = normalize(mul((float3x3) model, input.normal));
    float3 L;
    
    switch (light.type)
    {
        case LIGHT_DIRECTIONAL:
            L = normalize(-light.dir);
            break;
        case LIGHT_POINT:
            L = -1 * normalize(light.pos - worldPos); //did the -1 multiplication as a quick fix
            break;
    };
    
    float NdotL = saturate(dot(N, L * -1));
    
    float3 diffuse = light.color.rgb * light.color.a * NdotL;
    
    output.diffuse = diffuse + light.ambient;
    
    //SPECULAR
    output.specular = 0;
    if (NdotL)
    {
        float3 R = reflect(-L, N);
    }
    
    
    return output;
}