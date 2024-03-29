#include "../Common/LightingUtils.hlsli"

struct PassData
{
	float4x4 ViewMatrix;
	float4x4 ProjectionMatrix;
	float4x4 ViewProjectionMatrix;
	float4x4 InverseViewProjectionMatrix;
	float3 CameraPositionW;
	float Padding;
	Light Lights[MAX_NUM_LIGHTS];
};