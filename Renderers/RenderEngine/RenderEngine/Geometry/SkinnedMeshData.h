#pragma once

#include <cstdint>
#include <vector>

#include "SkinnedVertex.h"

namespace RenderEngine
{
	struct SkinnedMeshData
	{
	public:
		using VertexType = SkinnedVertex;
		using IndexType = std::uint32_t;

	public:
		std::vector<VertexType> Vertices;
		std::vector<IndexType> Indices;
	};
}
