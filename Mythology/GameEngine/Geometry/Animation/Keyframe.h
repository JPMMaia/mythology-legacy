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
			Scale({ 1.0f, 1.0f, 1.0f }),
			TimePosition(0.0f)
		{
		}

	public:
		Vector3 Translation;
		Quaternion Rotation;
		Vector3 Scale;
		float TimePosition;
	};
}