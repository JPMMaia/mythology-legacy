#pragma once

#include <DirectXMath.h>
#include <vector>

#include "MeshGenerator.h"

namespace DirectX12Engine
{
	namespace VertexTypes
	{
		struct PositionVertex
		{
			DirectX::XMFLOAT3 PositionL;

			static std::vector<PositionVertex> CreateFromMeshData(const MeshGenerator::MeshData& meshData);
		};

		struct PositionNormalVertex
		{
			DirectX::XMFLOAT3 PositionL;
			DirectX::XMFLOAT3 NormalL;

			static std::vector<PositionNormalVertex> CreateFromMeshData(const MeshGenerator::MeshData& meshData);
		};

		struct PositionNormalTextureCoordinatesVertex
		{
			DirectX::XMFLOAT3 PositionL;
			DirectX::XMFLOAT3 NormalL;
			DirectX::XMFLOAT2 TextureCoordinates;

			static std::vector<PositionNormalTextureCoordinatesVertex> CreateFromMeshData(const MeshGenerator::MeshData& meshData);
		};

		struct PositionTextureCoordinatesVextex
		{
			DirectX::XMFLOAT3 PositionL;
			DirectX::XMFLOAT2 TextureCoordinates;

			static std::vector<PositionTextureCoordinatesVextex> CreateFromMeshData(const MeshGenerator::MeshData& meshData);
		};

		using DefaultVertexType = PositionVertex;
	}
}
