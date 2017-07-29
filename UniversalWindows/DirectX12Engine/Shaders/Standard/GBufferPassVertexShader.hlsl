#include "../Common/InstanceData.hlsli"
#include "../Common/PassData.hlsli"
#include "../Common/MaterialData.hlsli"

struct VertexInput
{
	float3 PositionL : POSITION;
	float3 NormalL : NORMAL;
};
struct VertexOutput
{
	float4 PositionH : SV_POSITION;
	nointerpolation uint MaterialIndex : MATERIAL_INDEX;
};

ConstantBuffer<PassData> g_passData : register(b0);
StructuredBuffer<InstanceData> g_instanceData : register(t0, space1);
StructuredBuffer<MaterialData> g_materialData : register(t1, space1);

VertexOutput main(VertexInput input, uint instanceID : SV_InstanceID)
{
	VertexOutput output;

	InstanceData instanceData = g_instanceData[instanceID];

	float4 positionW = mul(float4(input.PositionL, 1.0f), instanceData.ModelMatrix);
	output.PositionH = mul(positionW, g_passData.ViewProjectionMatrix);

	output.MaterialIndex = instanceData.MaterialIndex;

	return output;
}
