#pragma once

#include <cstdint>
#include <vector>

#include "Vertex.h"

namespace RenderEngine
{
	struct MeshData
	{
	public:
		using VertexType = Vertex;
		using IndexType = std::uint32_t;

	public:
		std::vector<VertexType> Vertices;
		std::vector<IndexType> Indices;
	};
}
