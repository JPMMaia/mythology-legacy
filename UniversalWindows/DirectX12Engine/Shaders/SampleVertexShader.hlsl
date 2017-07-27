#include "InstanceData.hlsli"
#include "PassData.hlsli"
#include "MaterialData.hlsli"

struct VertexInput
{
	float3 PositionL : POSITION;
};
struct VertexOutput
{
	float4 PositionH : SV_POSITION;
	nointerpolation uint MaterialIndex : MATERIAL_INDEX;
};

ConstantBuffer<PassData> gPassData : register(b0);
StructuredBuffer<InstanceData> gInstanceData : register(t0, space1);
StructuredBuffer<MaterialData> gMaterialData : register(t1, space1);

VertexOutput main(VertexInput input, uint instanceID : SV_InstanceID)
{
	VertexOutput output;

	InstanceData instanceData = gInstanceData[instanceID];

	float4 positionW = mul(float4(input.PositionL, 1.0f), instanceData.ModelMatrix);
	output.PositionH = mul(positionW, gPassData.ViewProjectionMatrix);

	output.MaterialIndex = instanceData.MaterialIndex;

	return output;
}