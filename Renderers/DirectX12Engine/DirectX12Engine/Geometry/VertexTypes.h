#pragma once

#include "RenderEngine/Geometry/MeshData.h"
#include "RenderEngine/Geometry/SkinnedMeshData.h"

namespace DirectX12Engine
{
	namespace VertexTypes
	{
		struct PositionVertex
		{
			Eigen::Vector3f PositionL;

			static std::vector<PositionVertex> CreateFromMeshData(const RenderEngine::MeshData& meshData);
		};

		struct PositionNormalVertex
		{
			Eigen::Vector3f PositionL;
			Eigen::Vector3f NormalL;

			static std::vector<PositionNormalVertex> CreateFromMeshData(const RenderEngine::MeshData& meshData);
		};

		struct PositionTextureCoordinatesVextex
		{
			Eigen::Vector3f PositionL;
			Eigen::Vector2f TextureCoordinates;

			static std::vector<PositionTextureCoordinatesVextex> CreateFromMeshData(const RenderEngine::MeshData& meshData);
		};

		struct PositionNormalTextureCoordinatesVertex
		{
			Eigen::Vector3f PositionL;
			Eigen::Vector3f NormalL;
			Eigen::Vector2f TextureCoordinates;

			static std::vector<PositionNormalTextureCoordinatesVertex> CreateFromMeshData(const RenderEngine::MeshData& meshData);
		};

		struct PositionNormalTextureCoordinatesSkinnedVertex
		{
		public:
			PositionNormalTextureCoordinatesSkinnedVertex() :
				Weights({ 0.0f, 0.0f, 0.0f }),
				BoneIndices({ 0, 0, 0, 0 })
			{
			}

		public:
			Eigen::Vector3f PositionL;
			Eigen::Vector3f NormalL;
			Eigen::Vector2f TextureCoordinates;
			std::array<float, 3> Weights;
			std::array<std::uint8_t, 4> BoneIndices;

		public:
			static std::vector<PositionNormalTextureCoordinatesSkinnedVertex> CreateFromMeshData(const RenderEngine::SkinnedMeshData& meshData);
		};

		using DefaultVertexType = PositionVertex;
	}
}
