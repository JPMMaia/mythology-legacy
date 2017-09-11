#pragma once

#include <array>

namespace DirectX12Engine
{
	namespace ShaderBufferTypes
	{
		struct InstanceData
		{
			Eigen::Affine3f ModelMatrix;
			std::uint32_t MaterialIndex;
			std::uint32_t Pad0;
			std::uint32_t Pad1;
			std::uint32_t Pad2;
		};

		struct MaterialData
		{
			Eigen::Vector4f BaseColor;
			std::uint32_t AlbedoMapIndex;
			std::uint32_t Pad0;
			std::uint32_t Pad1;
			std::uint32_t Pad2;
		};

		struct LightData
		{
			Eigen::Vector3f Strength = { 0.5f, 0.5f, 0.5f };		// Light color
			float FalloffStart = 1.0f;								// Point/Spot light only
			Eigen::Vector3f Direction = { 0.0f, -1.0f, 0.0f };	// Directional/Spot light only
			float FalloffEnd = 10.0f;								// Point/Spot light only
			Eigen::Vector3f Position = { 0.0f, 0.0f, 0.0f };		// Point/Spot light only
			float SpotPower = 64.0f;								// Spot light only
		};

		struct alignas(256) PassData
		{
			Eigen::Affine3f ViewMatrix;
			Eigen::Affine3f ProjectionMatrix;
			Eigen::Affine3f ViewProjectionMatrix;
			Eigen::Affine3f InverseViewProjectionMatrix;
			Eigen::Vector3f CameraPositionW;
			float Padding;

			static constexpr std::size_t MaxNumLights = 8;
			std::array<LightData, MaxNumLights> Lights;
		};

		struct alignas(256) SkinnedAnimationData
		{
			static constexpr std::size_t MaxNumBones = 96;
			std::array<Eigen::Affine3f, MaxNumBones> BoneTransforms;
			Eigen::Affine3f ModelMatrix;
		};

		struct alignas(256) SkinnedMeshData
		{
			std::uint32_t MaterialIndex;
			std::uint32_t Pad0;
			std::uint32_t Pad1;
			std::uint32_t Pad2;
		};
	}
}
