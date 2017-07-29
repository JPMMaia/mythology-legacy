#include "../Common/MaterialData.hlsli"

struct PixelInput
{
	float4 PositionH : SV_POSITION;
	float3 NormalW : NORMAL;
	nointerpolation uint MaterialIndex : MATERIAL_INDEX;
};
struct PixelOutput
{
	float4 Albedo : SV_TARGET0;
	float4 NormalAndRoughness : SV_TARGET1;
};

StructuredBuffer<MaterialData> g_materialData : register(t1, space1);

PixelOutput main(PixelInput input)
{
	PixelOutput output;

	// Get the data of the instance's material:
	MaterialData materialData = g_materialData[input.MaterialIndex];

	// Output values:
	output.Albedo = materialData.BaseColor;
	output.NormalAndRoughness = float4(normalize(input.NormalW), 1.0f);

	return output;
}
