struct Input
{
};

struct Output
{
	float4 Color : SV_Target0;
};

Output main(Input input)
{
	Output output;
    output.Color = float4(0.6901960784f, 0.0431372549f, 0.4078431373f, 1.0f);

	return output;
}
