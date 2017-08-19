﻿#pragma once

#include "GameEngine/Geometry/Animation/SkinnedData.h"
#include "Common/Timer.h"
#include "GameEngine/Geometry/EigenGeometry.h"

namespace GameEngine
{
	class SkinnedModelInstance
	{
	public:
		SkinnedModelInstance() = default;
		explicit SkinnedModelInstance(SkinnedData&& skinnedData);

	public:
		void FrameUpdate(const Common::Timer& timer);

	public:
		const std::string& GetCurrentAnimationClip() const;
		void SetCurrentAnimationClip(const std::string& currentAnimationClipName);

		const std::vector<Eigen::Affine3f>& GetFinalTransforms() const;

	private:
		SkinnedData m_skinnedData;
		std::vector<Eigen::Affine3f> m_finalTransforms;
		std::string m_currentAnimationClipName;
		float m_timePosition = 0.0f;
	};
}