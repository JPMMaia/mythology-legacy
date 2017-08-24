#include "AnimationClip.h"

using namespace GameEngine;

AnimationClip::AnimationClip(const std::vector<BoneAnimation>& boneAnimations) :
	m_boneAnimations(boneAnimations)
{
	// TODO undo
	using Vector3 = Eigen::Vector3f;
	using Quaternion = Eigen::Quaternionf;

	for (std::size_t i = 0; i < boneAnimations.size(); ++i)
	{
		std::vector<Keyframe<Vector3>> positions(boneAnimations.size(), Keyframe<Vector3>(0.0f, Vector3(0.0f, 0.0f, 0.0f)));
		std::vector<Keyframe<Quaternion>> rotations(boneAnimations.size(), Keyframe<Quaternion>(0.0f, Quaternion(1.0f, 0.0f, 0.0f, 0.0f)));
		std::vector<Keyframe<Vector3>> scales(boneAnimations.size(), Keyframe<Vector3>(0.0f, Vector3(1.0f, 1.0f, 1.0f)));
		m_boneAnimations[i] = BoneAnimation(positions, rotations, scales);
	}

	std::vector<Keyframe<Vector3>> positions(boneAnimations.size(), Keyframe<Vector3>(0.0f, Vector3(0.0f, 0.0f, -350.0f)));
	std::vector<Keyframe<Quaternion>> rotations(boneAnimations.size(), Keyframe<Quaternion>(0.0f, Quaternion(1.0f, 0.0f, 0.0f, 0.0f)));
	std::vector<Keyframe<Vector3>> scales(boneAnimations.size(), Keyframe<Vector3>(0.0f, Vector3(1.0f, 1.0f, 1.0f)));
	m_boneAnimations[0] = BoneAnimation(positions, rotations, scales);
}

void AnimationClip::Interpolate(float timePosition, std::vector<Eigen::Affine3f>& boneTransforms) const
{
	boneTransforms.resize(m_boneAnimations.size());
	for (std::size_t i = 0; i < m_boneAnimations.size(); ++i)
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

	for (const auto& boneAnimation : m_boneAnimations)
		time = (std::max)(time, boneAnimation.GetEndTime());

	return time;
}
