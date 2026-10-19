cbuffer Uniforms : register(b0, space1)
{
    float4x4 mvp;
};


struct Input
{
    float3 position : TEXCOORD0;
    float2 texcoord : TEXCOORD1;
};

struct Output
{
    float4 Position : SV_Position;
    float2 Texcoord : TEXCOORD0;
};

Output main(Input input)
{
    Output output;
    output.Texcoord = input.texcoord;
    output.Position = mul(mvp, float4(input.position, 1.0f));

    return output;
}