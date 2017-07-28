#include "../Common/MaterialData.hlsli"

struct PixelInput
{
	float4 PositionH : SV_POSITION;
	nointerpolation uint MaterialIndex : MATERIAL_INDEX;
};
struct PixelOutput
{
	float4 Albedo : SV_TARGET0;
};

StructuredBuffer<MaterialData> g_materialData : register(t1, space1);

PixelOutput main(PixelInput input)
{
	PixelOutput output;

	MaterialData materialData = g_materialData[input.MaterialIndex];
	output.Albedo = materialData.BaseColor;

	return output;
}
