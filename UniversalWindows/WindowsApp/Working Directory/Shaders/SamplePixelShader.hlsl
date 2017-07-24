#include "MaterialData.hlsli"

struct PixelInput
{
	float4 PositionH : SV_POSITION;
	nointerpolation uint MaterialIndex : MATERIAL_INDEX;
};

StructuredBuffer<MaterialData> gMaterialData : register(t1, space1);

float4 main(PixelInput input) : SV_TARGET
{
	MaterialData materialData = gMaterialData[input.MaterialIndex];

	return materialData.BaseColor;
}