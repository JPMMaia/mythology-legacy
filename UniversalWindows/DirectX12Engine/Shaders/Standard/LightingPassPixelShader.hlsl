#include "../Common/Samplers.hlsli"
#include "../Common/PassData.hlsli"

struct PixelInput
{
	float4 PositionH : SV_POSITION;
	float2 TextureCoordinates : TEXCOORD0;
};

ConstantBuffer<PassData> g_passData : register(b0);
Texture2D g_positions : register(t0, space0);
Texture2D g_baseColorAndMetallicness : register(t1, space0);
Texture2D g_normalsAndRoughness : register(t2, space0);

float4 main(PixelInput input) : SV_TARGET
{
	// Sample position:
	float3 positionW = g_positions.Sample(g_samplerPointClamp, input.TextureCoordinates).xyz;

	// Sample base color and metallicness:
	float4 baseColorAndMetallicness = g_baseColorAndMetallicness.Sample(g_samplerPointClamp, input.TextureCoordinates);
	float3 baseColor = baseColorAndMetallicness.rgb;
	float metallicness = baseColorAndMetallicness.a; // TODO check gamma correction

	// Sample normal and roughness:
	float4 normalAndRoughness = g_normalsAndRoughness.Sample(g_samplerPointClamp, input.TextureCoordinates);
	float3 normalW = normalize(normalAndRoughness.xyz);
	float roughness = normalAndRoughness.w;

	Material material;
	material.DiffuseAlbedo = float4(baseColor, 1.0f);
	material.FresnelR0 = float3(0.1f, 0.1f, 0.1f);
	material.Shininess = 1.0f - roughness;

	float3 toEyeDirection = normalize(g_passData.CameraPositionW - positionW);
	float shadowFactor = 1.0f;
	float specularFactor = 1.0f;
	float4 color = ComputeLighting(g_passData.Lights, material, positionW, normalW, toEyeDirection, shadowFactor, specularFactor);

	return color;
}