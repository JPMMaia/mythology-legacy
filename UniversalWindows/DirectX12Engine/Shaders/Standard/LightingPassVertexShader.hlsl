struct VertexInput
{
	float3 PositionL : POSITION;
	float2 TextureCoordinates : TEXCOORD0;
};
struct VertexOutput
{
	float4 PositionH : SV_POSITION;
	float2 TextureCoordinates : TEXCOORD0;
};

VertexOutput main(VertexInput input)
{
	VertexOutput output;

	// Input position already specified in NDC system:
	output.PositionH = float4(input.PositionL, 1.0f);

	// Output texture coordinates:
	output.TextureCoordinates = input.TextureCoordinates;

	return output;
}