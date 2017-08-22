#include "SkinnedModelInstance.h"

using namespace GameEngine;

SkinnedModelInstance::SkinnedModelInstance(SkinnedData&& skinnedData) :
	m_skinnedData(std::move(skinnedData)),
	m_currentAnimationClipName(m_skinnedData.GetDefaultAnimationClipName())
{
}

void SkinnedModelInstance::FrameUpdate(const Common::Timer& timer)
{
	using milliseconds = std::chrono::duration<float, std::milli>;

	/*m_timePosition += milliseconds(timer.GetDeltaTime()).count();

	if (m_timePosition > m_skinnedData.GetClipEndTime(m_currentAnimationClipName))
		m_timePosition = 0.0f;

	// TODO change from 0.0f to m_timePosition
	m_skinnedData.GetFinalTransforms(m_currentAnimationClipName, 0.0f, m_finalTransforms);*/
	m_finalTransforms.resize(96, Eigen::Affine3f::Identity());
}

const std::string& SkinnedModelInstance::GetCurrentAnimationClip() const
{
	return m_currentAnimationClipName;
}
void SkinnedModelInstance::SetCurrentAnimationClip(const std::string& currentAnimationClipName)
{
	m_currentAnimationClipName = currentAnimationClipName;
}

const std::vector<Eigen::Affine3f>& SkinnedModelInstance::GetFinalTransforms() const
{
	return m_finalTransforms;
}
