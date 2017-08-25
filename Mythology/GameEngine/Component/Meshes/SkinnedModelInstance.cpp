#include "SkinnedModelInstance.h"

using namespace GameEngine;

SkinnedModelInstance::SkinnedModelInstance(Armature&& armature) :
	m_armature(std::move(armature)),
	m_currentAnimationClipName(m_armature.GetDefaultAnimationClipName())
{
}

void SkinnedModelInstance::FrameUpdate(const Common::Timer& timer)
{
	using milliseconds = std::chrono::duration<float, std::milli>;

	m_timePosition += milliseconds(timer.GetDeltaTime()).count();

	if (m_timePosition > m_armature.GetClipEndTime(m_currentAnimationClipName))
		m_timePosition = 0.0f;

	m_armature.GetFinalTransforms(m_currentAnimationClipName, m_timePosition, m_finalTransforms);
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
