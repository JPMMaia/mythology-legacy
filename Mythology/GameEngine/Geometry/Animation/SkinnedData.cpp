#include "SkinnedData.h"

using namespace GameEngine;

float SkinnedData::GetClipStartTime(const std::string& clipName) const
{
}
float SkinnedData::GetClipEndTime(const std::string& clipName) const
{
}
std::size_t SkinnedData::GetBoneCount() const
{
}
void SkinnedData::GetFinalTransforms(const std::string& clipName, float timePosition, std::vector<Eigen::Affine3f>& finalTransforms) const
{
	std::size_t boneCount = m_boneOffsets.size();

	std::vector<Eigen::Affine3f> toParentTransforms(boneCount);
	auto clip = m_animations.find(clipName);
	clip->second.Interpolate(timePosition, toParentTransforms);

	std::vector<Eigen::Affine3f> toRootTransforms(boneCount);
	toRootTransforms[0] = toParentTransforms[0];
	for(std::size_t i = 1; i < boneCount; ++i)
	{
		const auto& toParent = toParentTransforms[i];
		
		auto parentIndex = m_boneHierarchy[i];
		const auto& parentToRoot = toRootTransforms[parentIndex];

		toRootTransforms[i] = parentToRoot * toParent;
	}

	for(std::size_t i = 0; i < boneCount; ++i)
	{
		finalTransforms[i] = toRootTransforms[i] * m_boneOffsets[i];
	}
}

void SkinnedData::Set(std::vector<std::int32_t>& boneHierarchy, std::vector<Eigen::Affine3f>& boneOffsets, std::unordered_map<std::string, AnimationClip>& animations)
{
}
