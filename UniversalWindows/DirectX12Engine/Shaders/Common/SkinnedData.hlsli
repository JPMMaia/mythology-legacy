struct SkinnedAnimationData
{
	float4x4 BoneTransforms[96];
	float4x4 ModelMatrix;
};
struct SkinnedMeshData
{
	uint MaterialIndex;
	uint Pad0;
	uint Pad1;
	uint Pad2;
};
