#pragma once

#include "AnimationClip.h"
#include "Libraries/Eigen/Geometry"

#include <unordered_map>

namespace GameEngine
{
	class SkinnedData
	{
	public:
		float GetClipStartTime(const std::string& clipName) const;
		float GetClipEndTime(const std::string& clipName) const;
		std::size_t GetBoneCount() const;
		void GetFinalTransforms(const std::string& clipName, float timePosition, std::vector<Eigen::Affine3f>& finalTransforms) const;

		void Set(std::vector<std::int32_t>& boneHierarchy, std::vector<Eigen::Affine3f>& boneOffsets, std::unordered_map<std::string, AnimationClip>& animations);

	private:
		std::vector<std::int32_t> m_boneHierarchy;
		std::vector<Eigen::Affine3f> m_boneOffsets;
		std::unordered_map<std::string, AnimationClip> m_animations;
	};
}
