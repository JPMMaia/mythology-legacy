#include "../Common/Samplers.hlsli"

struct PixelInput
{
	float4 PositionH : SV_POSITION;
	float2 TextureCoordinates : TEXCOORD0;
};

Texture2D g_albedo;

float4 main(PixelInput input) : SV_TARGET
{
	float4 albedo = g_albedo.Sample(g_samplerPointClamp, input.TextureCoordinates);

	return albedo;
}