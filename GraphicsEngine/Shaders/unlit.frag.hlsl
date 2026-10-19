cbuffer Uniforms : register(b0, space3)
{
    float4 color;
};


struct Input
{
    float2 texcoord : TEXCOORD0;
};

struct Output
{
	float4 Color : SV_Target0;
};

Output main(Input input)
{
	Output output;
    output.Color = color;

	return output;
}
