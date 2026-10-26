#pragma once

#define LIGHT_DIRECTIONAL   0
#define LIGHT_POINT         1
#define LIGHT_SPOT          2

struct Material
{
    float2 tiling;
    float2 offset;
    
};

struct Light
{
    float3 pos;
    uint type;
    float3 dir;
    float padding2;
    float4 color;
    float3 ambient;
    float padding3;
};