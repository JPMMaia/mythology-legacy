#pragma once

#include "GameEngine/Geometry/EigenGeometry.h"

#include <vector>

namespace DirectX12Engine
{
	namespace VertexTypes
	{
		struct PositionVertex
		{
			Eigen::Vector3f PositionL;

			static std::vector<PositionVertex> CreateFromMeshData(const GameEngine::EigenMeshData& meshData);
		};

		struct PositionNormalVertex
		{
			Eigen::Vector3f PositionL;
			Eigen::Vector3f NormalL;

			static std::vector<PositionNormalVertex> CreateFromMeshData(const GameEngine::EigenMeshData& meshData);
		};

		struct PositionNormalTextureCoordinatesVertex
		{
			Eigen::Vector3f PositionL;
			Eigen::Vector3f NormalL;
			Eigen::Vector2f TextureCoordinates;

			static std::vector<PositionNormalTextureCoordinatesVertex> CreateFromMeshData(const GameEngine::EigenMeshData& meshData);
		};

		struct PositionTextureCoordinatesVextex
		{
			Eigen::Vector3f PositionL;
			Eigen::Vector2f TextureCoordinates;

			static std::vector<PositionTextureCoordinatesVextex> CreateFromMeshData(const GameEngine::EigenMeshData& meshData);
		};

		using DefaultVertexType = PositionVertex;
	}
}
