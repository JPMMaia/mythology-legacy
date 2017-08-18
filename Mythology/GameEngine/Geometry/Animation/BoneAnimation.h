#pragma once

#include "Keyframe.h"

#include <vector>

namespace GameEngine
{
	class BoneAnimation
	{
	public:
		BoneAnimation() = default;
		explicit BoneAnimation(const std::vector<Keyframe>& keyframes);

	public:
		void Interpolate(float timePosition, Eigen::Affine3f& matrix) const;

	public:
		float GetStartTime() const;
		float GetEndTime() const;

	private:
		std::vector<Keyframe> m_keyframes;
	};
}
