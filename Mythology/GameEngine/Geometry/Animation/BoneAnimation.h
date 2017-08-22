#pragma once

#include "Keyframe.h"

#include <vector>

namespace GameEngine
{
	class BoneAnimation
	{
	public:
		using Vector3 = Eigen::Vector3f;
		using Quaternion = Eigen::Quaternionf;

	public:
		BoneAnimation() = default;
		explicit BoneAnimation(const std::vector<Keyframe<Vector3>>& positionKeyframes, const std::vector<Keyframe<Quaternion>>& rotationKeyframes, const std::vector<Keyframe<Vector3>>& scalingKeyframes);

	public:
		void Interpolate(float timePosition, Eigen::Affine3f& matrix) const;

	public:
		float GetStartTime() const;
		float GetEndTime() const;

	private:
		std::vector<Keyframe<Vector3>> m_positionKeyframes;
		std::vector<Keyframe<Quaternion>> m_rotationKeyframes;
		std::vector<Keyframe<Vector3>> m_scaleKeyframes;
	};
}
