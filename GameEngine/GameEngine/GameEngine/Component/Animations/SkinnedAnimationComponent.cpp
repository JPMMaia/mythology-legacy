#include "pch.h"
#include "SkinnedAnimationComponent.h"

using namespace GameEngine;

SkinnedAnimationComponent::SkinnedAnimationComponent(const std::shared_ptr<Armature>& armature, const Eigen::Affine3f& meshToParentOfBoneRoot) :
	m_armature(armature),
	m_currentAnimationClipName(m_armature->GetDefaultAnimationClipName()),
	m_meshToParentOfBoneRoot(meshToParentOfBoneRoot)
{
}

void SkinnedAnimationComponent::FrameUpdate(const Common::Timer& timer)
{
	using milliseconds = std::chrono::duration<float, std::milli>;

	m_timePosition += milliseconds(timer.GetDeltaTime()).count();

	if (m_timePosition > m_armature->GetClipEndTime(m_currentAnimationClipName))
		m_timePosition = 0.0f;

	m_armature->GetFinalTransforms(m_currentAnimationClipName, m_timePosition, m_meshToParentOfBoneRoot, m_finalTransforms);
}

const std::string& SkinnedAnimationComponent::GetCurrentAnimationClip() const
{
	return m_currentAnimationClipName;
}
void SkinnedAnimationComponent::SetCurrentAnimationClip(const std::string& currentAnimationClipName)
{
	m_currentAnimationClipName = currentAnimationClipName;
}

const std::vector<Eigen::Affine3f>& SkinnedAnimationComponent::GetFinalTransforms() const
{
	return m_finalTransforms;
}
