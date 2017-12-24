#pragma once

#include "GameEngine/Geometry/Animation/Armature.h"
#include "Common/Timers/Timer.h"
#include "GameEngine/Geometry/EigenGeometry.h"
#include "GameEngine/Memory/UseStorage.h"
#include "GameEngine/Component/Base/BaseComponent.h"

#include <memory>

namespace GameEngine
{
	class SkinnedAnimationComponent : public IComponent
	{
	public:
		SkinnedAnimationComponent() = default;
		explicit SkinnedAnimationComponent(const std::shared_ptr<Armature>& armature, const Eigen::Affine3f& meshToParentOfBoneRoot);

	public:
		void FixedUpdate(const Common::Timer& timer) override;
		void SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldTransformStays = false) override;
		void UnsetParent(bool worldTransformStays = false) override;

	public:
		void FrameUpdate(const Common::Timer& timer);

	public:
		const std::string& GetCurrentAnimationClip() const;
		void SetCurrentAnimationClip(const std::string& currentAnimationClipName);

		const std::vector<Eigen::Affine3f>& GetFinalTransforms() const;

	private:
		std::shared_ptr<Armature> m_armature;
		std::vector<Eigen::Affine3f> m_finalTransforms;
		std::string m_currentAnimationClipName;
		float m_timePosition = 0.0f;
		Eigen::Affine3f m_meshToParentOfBoneRoot;
	};
}
