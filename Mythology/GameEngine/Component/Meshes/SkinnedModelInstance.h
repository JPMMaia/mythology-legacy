#pragma once

#include "GameEngine/Geometry/Animation/SkinnedData.h"
#include "Common/Timer.h"
#include "GameEngine/Geometry/EigenGeometry.h"
#include "GameEngine/Geometry/Primitives/CustomGeometry.h"
#include "MeshComponent.h"

#include <memory>

namespace GameEngine
{
	class SkinnedModelInstance
	{
	public:

	public:
		void FrameUpdate(const Common::Timer& timer);

	private:
		GameEngine::SkinnedData m_skinnedData;
		std::vector<Eigen::Affine3f> m_finalTransforms;
		std::string m_currentAnimationClipName;
		float m_timePosition = 0.0f;
	};
}
