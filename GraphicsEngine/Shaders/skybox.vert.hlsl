#include "Uniforms.hlsli"

cbuffer FrameUniforms : register(FRAME_SLOT, VERTEX_UNIFORM_SPACE)
{
    Frame frame;
};
struct Input
{
    float3 position : TEXCOORD0;
};
struct Output
{
    float3 texCoord : TEXCOORD0;
    float4 position : SV_Position;
};
Output main(Input input)
{
    Output output;
    output.texCoord = input.position;
    float3 dir = mul((float3x3) frame.view, input.position);
    float4 pos = mul(frame.projection, float4(dir, 1.0));
    output.position = pos.xyww;
    return output;
}
