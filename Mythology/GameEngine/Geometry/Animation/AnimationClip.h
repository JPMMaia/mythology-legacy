#pragma once

#include <vector>

namespace GameEngine
{
	class AnimationClip
	{
	public:
		float GetClipStartTime() const;
		float GetClipEndTime() const;

		void Interpolate(float time, std::vector<Eigen::Affine3f>& boneTransforms) const;

	private:
		std::vector<BoneAnimation> m_boneAnimations;
	};
}