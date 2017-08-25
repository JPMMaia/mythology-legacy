#pragma once

#include "AnimationClip.h"
#include "Libraries/Eigen/Geometry"

#include <unordered_map>

namespace GameEngine
{
	class Armature
	{
	public:
		Armature() = default;
		Armature(const std::vector<std::int8_t>& boneHierarchy, const std::vector<Eigen::Affine3f>& boneTransforms, const std::unordered_map<std::string, AnimationClip>& animations);

	public:
		float GetClipStartTime(const std::string& clipName) const;
		float GetClipEndTime(const std::string& clipName) const;
		std::size_t GetBoneCount() const;
		const std::string& GetDefaultAnimationClipName() const;
		void GetFinalTransforms(const std::string& clipName, float timePosition, std::vector<Eigen::Affine3f>& finalTransforms) const;

	private:
		std::vector<std::int8_t> m_boneHierarchy;
		std::vector<Eigen::Affine3f> m_boneTransforms;
		std::unordered_map<std::string, AnimationClip> m_animations;
	};
}
