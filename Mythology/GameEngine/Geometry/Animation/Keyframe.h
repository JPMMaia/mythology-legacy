#pragma once

#include "Libraries/Eigen/Geometry"

namespace GameEngine
{
	struct Keyframe
	{
	public:
		using Vector3 = Eigen::Vector3f;
		using Quaternion = Eigen::Quaternionf;

	public:
		Keyframe() :
			Translation({ 0.0f, 0.0f, 0.0f }),
			Rotation({ 0.0f, 0.0f, 0.0f, 1.0f }),
			Scaling({ 1.0f, 1.0f, 1.0f }),
			TimePosition(0.0f)
		{
		}
		Keyframe(const Vector3& translation, const Quaternion& rotation, const Vector3& scale, float timePosition) :
			Translation(translation),
			Rotation(rotation),
			Scaling(scale),
			TimePosition(timePosition)
		{
		}

	public:
		Vector3 Translation;
		Quaternion Rotation;
		Vector3 Scaling;
		float TimePosition;
	};
}