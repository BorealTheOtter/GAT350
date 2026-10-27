#pragma once

#define LIGHT_DIRECTIONAL   0
#define LIGHT_POINT         1
#define LIGHT_SPOT          2

struct Material
{
    float4 color;
    float2 tiling;
    float2 offset;
    float shine;
    
};

struct Light
{
    float3 pos;
    uint type;
    float3 dir;
    float innerCutoff;
    float4 color;
    float3 ambient;
    float outerCutoff;
};

float3 getLightVector(Light light, float3 pos, out float attenuation)
{
    float3 L;
    attenuation = 1.0f;
    
    switch (light.type)
    {
        case LIGHT_DIRECTIONAL:
            L = normalize(light.dir);
            break;
        case LIGHT_POINT:
            L = normalize(light.pos - pos);
            break;
        case LIGHT_SPOT:
            L = normalize(light.pos - pos);
            attenuation = smoothstep(light.outerCutoff, light.innerCutoff, dot(-L, light.dir));
            break;
    };
    return L;
}