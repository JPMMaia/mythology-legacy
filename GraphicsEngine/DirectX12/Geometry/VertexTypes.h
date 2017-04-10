#pragma once

#include <DirectXMath.h>
#include <vector>

#include "MeshGenerator.h"

namespace GraphicsEngine
{
	namespace VertexTypes
	{
		struct PositionVertex
		{
			DirectX::XMFLOAT3 PositionL;

			static std::vector<PositionVertex> CreateFromMeshData(const MeshGenerator::MeshData& meshData);
		};

		using DefaultVertexType = PositionVertex;
	}
}
