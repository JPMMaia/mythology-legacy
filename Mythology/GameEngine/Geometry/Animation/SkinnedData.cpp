#include "SkinnedData.h"

using namespace GameEngine;

SkinnedData::SkinnedData(const std::vector<std::int32_t>& boneHierarchy, const std::vector<Eigen::Affine3f>& boneOffsets, const std::unordered_map<std::string, AnimationClip>& animations) :
	m_boneHierarchy(boneHierarchy),
	m_boneOffsets(boneOffsets),
	m_animations(animations)
{
}

float SkinnedData::GetClipStartTime(const std::string& clipName) const
{
	const auto& clip = m_animations.at(clipName);
	return clip.GetClipStartTime();
}
float SkinnedData::GetClipEndTime(const std::string& clipName) const
{
	const auto& clip = m_animations.at(clipName);
	return clip.GetClipEndTime();
}
std::size_t SkinnedData::GetBoneCount() const
{
	return m_boneHierarchy.size();
}
void SkinnedData::GetFinalTransforms(const std::string& clipName, float timePosition, std::vector<Eigen::Affine3f>& finalTransforms) const
{
	std::size_t boneCount = m_boneOffsets.size();

	std::vector<Eigen::Affine3f> toParentTransforms(boneCount);
	auto clip = m_animations.at(clipName);
	clip.Interpolate(timePosition, toParentTransforms);

	std::vector<Eigen::Affine3f> toRootTransforms(boneCount);
	toRootTransforms[0] = toParentTransforms[0];
	for (std::size_t i = 1; i < boneCount; ++i)
	{
		const auto& toParent = toParentTransforms[i];

		auto parentIndex = m_boneHierarchy[i];
		const auto& parentToRoot = toRootTransforms[parentIndex];

		toRootTransforms[i] = parentToRoot * toParent;
	}

	for (std::size_t i = 0; i < boneCount; ++i)
	{
		finalTransforms[i] = toRootTransforms[i] * m_boneOffsets[i];
	}
}
