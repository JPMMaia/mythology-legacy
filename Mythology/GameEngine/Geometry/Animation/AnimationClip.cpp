#include "AnimationClip.h"

using namespace GameEngine;

void AnimationClip::Interpolate(float timePosition, std::vector<Eigen::Affine3f>& boneTransforms) const
{
	for(std::size_t i = 0; i < m_boneAnimations.size(); ++i)
	{
		m_boneAnimations[i].Interpolate(timePosition, boneTransforms[i]);
	}
}

float AnimationClip::GetClipStartTime() const
{
	auto time = 0.0f;

	for (const auto& boneAnimation : m_boneAnimations)
		time = (std::min)(time, boneAnimation.GetStartTime());

	return time;
}
float AnimationClip::GetClipEndTime() const
{
	auto time = 0.0f;

	for(const auto& boneAnimation : m_boneAnimations)
		time = (std::max)(time, boneAnimation.GetEndTime());

	return time;
}
