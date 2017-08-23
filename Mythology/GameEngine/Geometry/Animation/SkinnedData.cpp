#include "SkinnedData.h"

using namespace GameEngine;

SkinnedData::SkinnedData(const std::vector<std::int8_t>& boneHierarchy, const std::vector<Eigen::Affine3f>& boneTransforms, const std::unordered_map<std::string, AnimationClip>& animations) :
	m_boneHierarchy(boneHierarchy),
	m_boneTransforms(boneTransforms),
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
const std::string& SkinnedData::GetDefaultAnimationClipName() const
{
	return m_animations.begin()->first;
}
void SkinnedData::GetFinalTransforms(const std::string& clipName, float timePosition, std::vector<Eigen::Affine3f>& finalTransforms) const
{
	auto boneCount = m_boneHierarchy.size();

	std::vector<Eigen::Affine3f> inverseBindPoseMatrices;
	{
		// Reserve memory:
		inverseBindPoseMatrices.reserve(boneCount);

		// Calculate the first offset:
		inverseBindPoseMatrices.emplace_back(m_boneTransforms.at(0).inverse());

		// Calculate the remaining offsets:
		for (std::size_t i = 1; i < boneCount; ++i)
		{
			const auto& parentOffset = inverseBindPoseMatrices.at(m_boneHierarchy[i]);
			inverseBindPoseMatrices.emplace_back(m_boneTransforms.at(i).inverse() * parentOffset);
		}
	}

	// Calculate the animation matrices:
	std::vector<Eigen::Affine3f> animationMatrices(boneCount);
	const auto& clip = m_animations.at(clipName);
	clip.Interpolate(timePosition, animationMatrices);
	
	// Calculate the final transforms:
	finalTransforms.reserve(boneCount);
	for (std::size_t i = 0; i < boneCount; ++i)
	{
		// TODO
	}

	/*
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

	finalTransforms.resize(boneCount);
	for (std::size_t i = 0; i < boneCount; ++i)
	{
		finalTransforms[i] = toRootTransforms[i] * m_boneOffsets[i];
	}*/
}
