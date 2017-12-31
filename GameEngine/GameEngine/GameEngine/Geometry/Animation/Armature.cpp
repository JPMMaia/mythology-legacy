#include "pch.h"
#include "Armature.h"

using namespace GameEngine;

Armature::Armature(const std::vector<std::int8_t>& boneHierarchy, const std::vector<Eigen::Affine3f>& boneTransforms, const std::unordered_map<std::string, AnimationClip>& animations) :
	m_boneHierarchy(boneHierarchy),
	m_animations(animations)
{
	auto boneCount = m_boneHierarchy.size();
	{
		// Reserve memory:
		m_inverseBindPoseTransforms.reserve(boneCount);

		// Calculate the first inverse bind pose matrix:
		m_inverseBindPoseTransforms.emplace_back(boneTransforms[0].inverse());

		// Calculate the remaining transforms:
		for (std::size_t i = 1; i < boneCount; ++i)
		{
			const auto& parentOffset = m_inverseBindPoseTransforms[m_boneHierarchy[i]];
			m_inverseBindPoseTransforms.emplace_back(boneTransforms[i].inverse() * parentOffset);
		}
	}
}

float Armature::GetClipStartTime(const std::string& clipName) const
{
	const auto& clip = m_animations.at(clipName);
	return clip.GetClipStartTime();
}
float Armature::GetClipEndTime(const std::string& clipName) const
{
	const auto& clip = m_animations.at(clipName);
	return clip.GetClipEndTime();
}
std::size_t Armature::GetBoneCount() const
{
	return m_boneHierarchy.size();
}
const std::string& Armature::GetDefaultAnimationClipName() const
{
	return m_animations.begin()->first;
}
void Armature::GetFinalTransforms(const std::string& clipName, float timePosition, const Eigen::Affine3f& meshToParentOfBoneRootMatrix, std::vector<Eigen::Affine3f>& finalTransforms) const
{
	auto boneCount = m_boneHierarchy.size();

	std::vector<Eigen::Affine3f> toParentTransforms;
	{
		const auto& clip = m_animations.at(clipName);
		clip.Interpolate(timePosition, toParentTransforms);
	}

	std::vector<Eigen::Affine3f> toRootTransforms;
	{
		toRootTransforms.reserve(boneCount);
		toRootTransforms.emplace_back(toParentTransforms[0]);
		for (std::size_t i = 1; i < boneCount; ++i)
		{
			const auto& toParentTransform = toParentTransforms[i];
			const auto& toRootTransform = toRootTransforms[m_boneHierarchy[i]];
			toRootTransforms.emplace_back(toRootTransform * toParentTransform);
		}
	}

	// Calculate the final transforms:
	finalTransforms.resize(boneCount);
	auto parentOfBoneRootToMeshMatrix = meshToParentOfBoneRootMatrix.inverse();
	for (std::size_t i = 0; i < boneCount; ++i)
	{
		finalTransforms[i] = parentOfBoneRootToMeshMatrix * toRootTransforms[i] * m_inverseBindPoseTransforms[i] * meshToParentOfBoneRootMatrix;
	}
}
