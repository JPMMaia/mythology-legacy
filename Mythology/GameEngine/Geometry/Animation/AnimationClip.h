#pragma once

#include "BoneAnimation.h"

namespace GameEngine
{
	class AnimationClip
	{
	public:
		AnimationClip() = default;
		explicit AnimationClip(const std::vector<BoneAnimation>& boneAnimations);

	public:
		void Interpolate(float timePosition, std::vector<Eigen::Affine3f>& boneTransforms) const;

	public:
		float GetClipStartTime() const;
		float GetClipEndTime() const;

	private:
		std::vector<BoneAnimation> m_boneAnimations;
	};
}
