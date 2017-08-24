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
			const auto& parentOffset = inverseBindPoseMatrices[m_boneHierarchy[i]];
			inverseBindPoseMatrices.emplace_back(m_boneTransforms[i].inverse() * parentOffset);
		}
	}
	
	std::vector<Eigen::Affine3f> parentTransforms;
	{
		// Calculate the animation matrices:
		std::vector<Eigen::Affine3f> animationMatrices;
		{
			const auto& clip = m_animations.at(clipName);
			clip.Interpolate(timePosition, animationMatrices);
		}

		// Calculate the parent transforms:
		parentTransforms.reserve(boneCount);
		parentTransforms.emplace_back(m_boneTransforms[0] * animationMatrices[0]);
		for (std::size_t i = 1; i < boneCount; ++i)
		{
			const auto& parentTransform = parentTransforms[m_boneHierarchy[i]];
			parentTransforms.emplace_back(parentTransform * m_boneTransforms[i] * animationMatrices[i]);
		}
	}

	// Calculate the final transforms:
	finalTransforms.resize(boneCount);
	for (std::size_t i = 0; i < boneCount; ++i)
	{
		finalTransforms[i] = parentTransforms[i] * inverseBindPoseMatrices[i];
	}
}
