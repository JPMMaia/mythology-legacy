#include "pch.h"
#include "BoneAnimation.h"
#include "Common/EigenHelpers.h"

using namespace Common;
using namespace GameEngine;

BoneAnimation::BoneAnimation(const std::vector<Keyframe<Vector3>>& positionKeyframes, const std::vector<Keyframe<Quaternion>>& rotationKeyframes, const std::vector<Keyframe<Vector3>>& scalingKeyframes) :
	m_positionKeyframes(positionKeyframes),
	m_rotationKeyframes(rotationKeyframes),
	m_scaleKeyframes(scalingKeyframes)
{
	auto endTime = GetEndTime();
	auto addStartAndEndKeyframes = [endTime](auto& keyframes, const auto& defaultValue)
	{
		if(keyframes.empty())
			keyframes.push_back({ 0.0f, defaultValue });
		if(keyframes.size() == 1)
			keyframes.push_back({ endTime, keyframes.front().Value });
	};

	addStartAndEndKeyframes(m_positionKeyframes, Eigen::Vector3f(0.0f, 0.0f, 0.0f));
	addStartAndEndKeyframes(m_rotationKeyframes, Eigen::Quaternionf::Identity());
	addStartAndEndKeyframes(m_scaleKeyframes, Eigen::Vector3f(1.0f, 1.0f, 1.0f));
}

void BoneAnimation::Interpolate(float timePosition, Eigen::Affine3f& matrix) const
{
	if (timePosition <= GetStartTime())
	{
		matrix.fromPositionOrientationScale(m_positionKeyframes.front().Value, m_rotationKeyframes.front().Value, m_scaleKeyframes.front().Value);
	}
	else if (timePosition >= GetEndTime())
	{
		matrix.fromPositionOrientationScale(m_positionKeyframes.back().Value, m_rotationKeyframes.back().Value, m_scaleKeyframes.back().Value);
	}
	else
	{
		Eigen::Vector3f position;
		{
			const auto& keyframes = m_positionKeyframes;
			for (std::size_t i = 0; i < keyframes.size() - 1; ++i)
			{
				const auto& keyframe = keyframes[i];
				const auto& followingKeyframe = keyframes[i + 1];

				if (keyframe.TimePosition <= timePosition && timePosition <= followingKeyframe.TimePosition)
				{
					auto percentage = (timePosition - keyframe.TimePosition) / (followingKeyframe.TimePosition - keyframe.TimePosition);
					position = EigenHelpers::LinearInterpolate(keyframe.Value, followingKeyframe.Value, percentage);
					break;
				}
			}
		}

		Eigen::Quaternionf rotation;
		{
			const auto& keyframes = m_rotationKeyframes;
			for (std::size_t i = 0; i < keyframes.size() - 1; ++i)
			{
				const auto& keyframe = keyframes[i];
				const auto& followingKeyframe = keyframes[i + 1];

				if (keyframe.TimePosition <= timePosition && timePosition <= followingKeyframe.TimePosition)
				{
					auto percentage = (timePosition - keyframe.TimePosition) / (followingKeyframe.TimePosition - keyframe.TimePosition);
					rotation = keyframe.Value.slerp(percentage, followingKeyframe.Value);
					break;
				}
			}
		}

		Eigen::Vector3f scale;
		{
			const auto& keyframes = m_scaleKeyframes;
			for (std::size_t i = 0; i < keyframes.size() - 1; ++i)
			{
				const auto& keyframe = keyframes[i];
				const auto& followingKeyframe = keyframes[i + 1];

				if (keyframe.TimePosition <= timePosition && timePosition <= followingKeyframe.TimePosition)
				{
					auto percentage = (timePosition - keyframe.TimePosition) / (followingKeyframe.TimePosition - keyframe.TimePosition);
					scale = EigenHelpers::LinearInterpolate(keyframe.Value, followingKeyframe.Value, percentage);
					break;
				}
			}
		}

		matrix.fromPositionOrientationScale(position, rotation, scale);
	}
}

float BoneAnimation::GetStartTime() const
{
	auto value = (std::numeric_limits<float>::max)();

	auto findMinimum = [&value](const auto& keyframes)
	{
		for (const auto& element : keyframes)
			value = (std::min)(value, element.TimePosition);
	};
	findMinimum(m_positionKeyframes);
	findMinimum(m_rotationKeyframes);
	findMinimum(m_scaleKeyframes);

	return value;
}
float BoneAnimation::GetEndTime() const
{
	auto value = 0.0f;

	auto findMaximum = [&value](const auto& keyframes)
	{
		for (const auto& element : keyframes)
			value = (std::max)(value, element.TimePosition);
	};
	findMaximum(m_positionKeyframes);
	findMaximum(m_rotationKeyframes);
	findMaximum(m_scaleKeyframes);

	return value;
}
