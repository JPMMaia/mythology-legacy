#include "../Common/Samplers.hlsli"
#include "../Common/PassData.hlsli"
#include "../Common/LightingUtils.hlsli"

struct PixelInput
{
	float4 PositionH : SV_POSITION;
	float2 TextureCoordinates : TEXCOORD0;
};

ConstantBuffer<PassData> g_passData : register(b0);
Texture2D g_positions : register(t0, space2);
Texture2D g_albedo : register(t1, space2);
Texture2D g_normals : register(t2, space2);

float4 main(PixelInput input) : SV_TARGET
{
	float3 positionW = g_positions.Sample(g_samplerPointClamp, input.TextureCoordinates).xyz;
	float4 albedo = g_albedo.Sample(g_samplerPointClamp, input.TextureCoordinates);
	float3 normalW = g_normals.Sample(g_samplerPointClamp, input.TextureCoordinates).xyz;

	Light lights[1];
	lights[0].Strength = float3(1.0f, 1.0f, 1.0f);
	lights[0].FalloffStart = 3.0f;
	lights[0].FalloffEnd = 8.0f;
	lights[0].Position = float3(2.0f, 3.0f, -2.0f);

	Material material;
	material.DiffuseAlbedo = albedo;
	material.FresnelR0 = float3(0.1f, 0.1f, 0.1f);
	material.Shininess = 1.0f;

	float3 toEyeDirection = normalize(g_passData.CameraPositionW - positionW);
	float shadowFactor = 1.0f;
	float specularFactor = 1.0f;
	float4 color = ComputeLighting(lights, material, positionW, normalW, toEyeDirection, shadowFactor, specularFactor);

	return color;
}