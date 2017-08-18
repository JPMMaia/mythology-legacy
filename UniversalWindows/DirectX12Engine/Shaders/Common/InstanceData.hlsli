struct InstanceData
{
	float4x4 ModelMatrix;
	uint MaterialIndex;
	uint Pad0;
	uint Pad1;
	uint Pad2;

#if defined(SKINNED)
	float4x4 BoneTransforms[30];
#endif

};