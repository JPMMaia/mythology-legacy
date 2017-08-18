#include "BoneAnimation.h"
#include "Common/EigenHelpers.h"

using namespace Common;
using namespace GameEngine;

BoneAnimation::BoneAnimation(const std::vector<Keyframe>& keyframes) :
	m_keyframes(keyframes)
{
}

void BoneAnimation::Interpolate(float timePosition, Eigen::Affine3f& matrix) const
{
	if(timePosition < GetStartTime())
	{
		const auto& keyframe =  m_keyframes.front();
		matrix.fromPositionOrientationScale(keyframe.Translation, keyframe.Rotation, keyframe.Scaling);
	}
	else if(timePosition > GetEndTime())
	{
		const auto& keyframe = m_keyframes.back();
		matrix.fromPositionOrientationScale(keyframe.Translation, keyframe.Rotation, keyframe.Scaling);
	}
	else
	{
		for(std::size_t i = 0; i < m_keyframes.size() - 1; ++i)
		{
			const auto& keyframe = m_keyframes[i];
			const auto& followingKeyframe = m_keyframes[i];

			if(keyframe.TimePosition <= timePosition && timePosition <= followingKeyframe.TimePosition)
			{
				auto percentage = (timePosition - keyframe.TimePosition) / (followingKeyframe.TimePosition - keyframe.TimePosition);
				
				auto translation = EigenHelpers::LinearInterpolate(keyframe.Translation, followingKeyframe.Translation, percentage);
				auto rotation = keyframe.Rotation.slerp(percentage, followingKeyframe.Rotation);
				auto scale = EigenHelpers::LinearInterpolate(keyframe.Scaling, followingKeyframe.Scaling, percentage);

				matrix.fromPositionOrientationScale(translation, rotation, scale);

				break;
			}
		}
	}
}

float BoneAnimation::GetStartTime() const
{
	return m_keyframes.front().TimePosition;
}
float BoneAnimation::GetEndTime() const
{
	return m_keyframes.back().TimePosition;
}
