#pragma once

#include "GameEngine/Geometry/Animation/Armature.h"
#include "Common/Timer.h"
#include "GameEngine/Geometry/EigenGeometry.h"

namespace GameEngine
{
	class SkinnedModelInstance
	{
	public:
		SkinnedModelInstance() = default;
		explicit SkinnedModelInstance(Armature&& armature, const Eigen::Affine3f& meshToBoneRoot);

	public:
		void FrameUpdate(const Common::Timer& timer);

	public:
		const std::string& GetCurrentAnimationClip() const;
		void SetCurrentAnimationClip(const std::string& currentAnimationClipName);

		const std::vector<Eigen::Affine3f>& GetFinalTransforms() const;

	private:
		Armature m_armature;
		std::vector<Eigen::Affine3f> m_finalTransforms;
		std::string m_currentAnimationClipName;
		float m_timePosition = 0.0f;
		Eigen::Affine3f m_meshToBoneRoot;
	};
}
