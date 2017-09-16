#include "../Common/MaterialData.hlsli"
#include "../Common/Samplers.hlsli"

#ifndef TEXTURE_COUNT
#define TEXTURE_COUNT 1
#endif

struct PixelInput
{
	float4 PositionH : SV_POSITION;
	float3 PositionW : POSITION;
	float3 NormalW : NORMAL;
	float2 TextureCoordinates : TEXCOORD0;
	nointerpolation uint MaterialIndex : MATERIAL_INDEX;
};
struct PixelOutput
{
	float4 PositionW : SV_TARGET0;
	float4 BaseColorAndMetallicness : SV_TARGET1;
	float4 NormalsAndRoughness : SV_TARGET2;
};

StructuredBuffer<MaterialData> g_materialData : register(t1, space1);
Texture2D g_baseColorTextures[TEXTURE_COUNT] : register(t0, space2);

PixelOutput main(PixelInput input)
{
	PixelOutput output;

	// Get the data of the instance's material:
	MaterialData materialData = g_materialData[input.MaterialIndex];

	// Output world position:
	output.PositionW = float4(input.PositionW, 1.0f);

	// Output base color and metallicness:
	float3 baseColor = materialData.BaseColorFactor.rgb * g_baseColorTextures[materialData.BaseColorTextureIndex].Sample(g_samplerLinearWrap, input.TextureCoordinates).rgb;
	float metallicness = materialData.MetallicFactor;
	output.BaseColorAndMetallicness = float4(baseColor, metallicness);

	// Output normal and roughness:
	float3 normal = input.NormalW;
	float roughness = materialData.RoughnessFactor;
	output.NormalsAndRoughness = float4(normal, roughness);

	return output;
}
