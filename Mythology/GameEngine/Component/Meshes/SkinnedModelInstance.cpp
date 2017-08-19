#include "SkinnedModelInstance.h"

using namespace GameEngine;

void SkinnedModelInstance::FrameUpdate(const Common::Timer& timer)
{
	using milliseconds = std::chrono::duration<float, std::milli>;
	
	m_timePosition += milliseconds(timer.GetDeltaTime()).count();

	if(m_timePosition > m_skinnedData.GetClipEndTime(m_currentAnimationClipName))
		m_timePosition = 0.0f;

	m_skinnedData.GetFinalTransforms(m_currentAnimationClipName, m_timePosition, m_finalTransforms);
}
