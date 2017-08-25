#include "Armature.h"

using namespace GameEngine;

Armature::Armature(const std::vector<std::int8_t>& boneHierarchy, const std::vector<Eigen::Affine3f>& boneTransforms, const std::unordered_map<std::string, AnimationClip>& animations) :
	m_boneHierarchy(boneHierarchy),
	m_boneTransforms(boneTransforms),
	m_animations(animations)
{
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
void Armature::GetFinalTransforms(const std::string& clipName, float timePosition, std::vector<Eigen::Affine3f>& finalTransforms) const
{
	//    (Root * B0 * B1 * B2)^-1 * V
	//    VL = (Mesh * Root * VG)
	//    VG = Root^(-1) * Mesh^(-1) * VL
	//    VB = B2 * B1 * B0 * Root * Root^(-1) * Mesh^(-1) * VL

	//               Root
	//                 |
	//             --------
	//             |       |
	//           Mesh      B0
	//                     |
	//                     B1
	//                     |
	//                     B2

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
