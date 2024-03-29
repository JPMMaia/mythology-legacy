#include "../Common/InstanceData.hlsli"
#include "../Common/PassData.hlsli"
#include "../Common/MaterialData.hlsli"
#include "../Common/SkinnedData.hlsli"

struct VertexInput
{
	float3 PositionL : POSITION;
	float3 NormalL : NORMAL;
	float2 TextureCoordinates : TEXCOORD0;

#if defined(SKINNED)
	float3 BoneWeights : WEIGHTS;
	uint4 BoneIndices : BONEINDICES;
#endif

};
struct VertexOutput
{
	float4 PositionH : SV_POSITION;
	float3 PositionW : POSITION;
	float3 NormalW : NORMAL;
	float2 TextureCoordinates : TEXCOORD0;
	nointerpolation uint MaterialIndex : MATERIAL_INDEX;
};

ConstantBuffer<PassData> g_passData : register(b0);
StructuredBuffer<MaterialData> g_materialData : register(t1, space1);

#if defined(SKINNED)
ConstantBuffer<SkinnedAnimationData> g_skinnedAnimationData : register(b1);
ConstantBuffer<SkinnedMeshData> g_skinnedMeshData : register(b2);
#else
StructuredBuffer<InstanceData> g_instanceData : register(t0, space1);
#endif

VertexOutput main(VertexInput input, uint instanceID : SV_InstanceID)
{
	VertexOutput output;

#if defined(SKINNED)
	float3 positionL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	{
		float weights[4] =
		{
			input.BoneWeights.x,
			input.BoneWeights.y,
			input.BoneWeights.z,
			1.0f - input.BoneWeights.x - input.BoneWeights.y - input.BoneWeights.z
		};

		for (uint i = 0; i < 4; ++i)
		{
			float4x4 boneTransform = g_skinnedAnimationData.BoneTransforms[input.BoneIndices[i]];
			positionL += weights[i] * mul(boneTransform, float4(input.PositionL, 1.0f)).xyz;
			normalL += weights[i] * mul((float3x3) boneTransform, input.NormalL);
		}
	}
#else
	float3 positionL = input.PositionL;
	float3 normalL = input.NormalL;
#endif

#if defined(SKINNED)
	float4x4 modelMatrix = g_skinnedAnimationData.ModelMatrix;
	uint materialIndex = g_skinnedMeshData.MaterialIndex;
#else
	// Get data of the instance:
	InstanceData instanceData = g_instanceData[instanceID];

	float4x4 modelMatrix = instanceData.ModelMatrix;
	uint materialIndex = instanceData.MaterialIndex;
#endif

	// Transfrom position from local space to world space:
	float4 positionW = mul(modelMatrix, float4(positionL, 1.0f));
	output.PositionW = positionW.xyz;

	// Transfrom position from world space to projection space:
	output.PositionH = mul(g_passData.ViewProjectionMatrix, positionW);

	// Transfrom normal from local space to world space, assuming that there is no non-uniform transformation:
	output.NormalW = mul((float3x3) modelMatrix, normalL);

	// Output texture coordinates:
	output.TextureCoordinates = input.TextureCoordinates;

	// Output the index of the instance's material:
	output.MaterialIndex = materialIndex;

	return output;
}
