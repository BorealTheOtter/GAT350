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
    float3 padding;
    
};

struct Light
{
    float3 pos;
    uint type;
    
    float3 dir;
    float range;
    
    float4 color;
    
    float3 ambient;
    float innerCutoff;
    
    float outerCutoff;
    float3 padding;
};

float calculateAttenuation(float distance, float range)
{
    return pow(1 - saturate(distance / range),2);
}

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
            {
            float3 toLight = light.pos - pos;
            float distance = length(toLight);
            attenuation = calculateAttenuation(distance, light.range);
            L = toLight / distance;
            }
            break;
        case LIGHT_SPOT:
            {
            float3 toLight = light.pos - pos;
            float distance = length(toLight);
            attenuation = calculateAttenuation(distance, light.range);
            L = toLight / distance;
            attenuation *= smoothstep(light.outerCutoff, light.innerCutoff, dot(-L, light.dir));
            }
            break;
    };
    return L;
}

float3 calculateDiffuse(float3 radiance, float3 N, float3 L)
{
    return radiance * saturate(dot(N, L));
}

float3 calculateSpecular(float3 radiance, float3 N, float3 L, float3 V, Material mat)
{
    float3 specular = 0;
    if (dot(N,L) > 0)
    {
        //Blinn-Phong Specular
        float3 H = normalize(V + L);
        float HdotN = saturate(dot(H, N));
        specular = pow(HdotN, mat.shine) * radiance;
    }
    
    return specular;
}

void calculateLighting(Light light, Material material, float3 position, float3 N, float3 V,
                       out float3 diffuse, out float3 specular)
{
    float attenuation;
    float3 L = getLightVector(light, position, attenuation);

    // light arriving at the surface
    float3 radiance = light.color.rgb * light.color.a * attenuation;

    diffuse = light.ambient + calculateDiffuse(radiance, N, L);
    specular = calculateSpecular(radiance, N, L, V, material);
}