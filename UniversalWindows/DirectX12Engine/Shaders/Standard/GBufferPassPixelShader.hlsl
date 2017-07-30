#include "../Common/MaterialData.hlsli"

struct PixelInput
{
	float4 PositionH : SV_POSITION;
	float3 PositionW : POSITION;
	float3 NormalW : NORMAL;
	nointerpolation uint MaterialIndex : MATERIAL_INDEX;
};
struct PixelOutput
{
	float4 PositionW : SV_TARGET0;
	float4 Albedo : SV_TARGET1;
	float4 NormalAndRoughness : SV_TARGET2;
};

StructuredBuffer<MaterialData> g_materialData : register(t1, space1);

PixelOutput main(PixelInput input)
{
	PixelOutput output;

	// Get the data of the instance's material:
	MaterialData materialData = g_materialData[input.MaterialIndex];

	// Output values:
	output.PositionW = float4(input.PositionW, 1.0f);
	output.Albedo = materialData.BaseColor;
	output.NormalAndRoughness = float4(normalize(input.NormalW), 1.0f);

	return output;
}
