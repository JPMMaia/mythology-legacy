#pragma once

#include "VertexData.h"
#include "MeshData.h"
#include "Libraries/Eigen/Core"
#include "MeshGenerator.h"

#include <cstddef>

namespace GameEngine
{
	using EigenVertex = Vertex<Eigen::Vector2f, Eigen::Vector3f>;
	using EigenMeshData = MeshData<Eigen::Vector2f, Eigen::Vector3f, std::uint32_t>;
	using EigenMeshGenerator = MeshGenerator<EigenMeshData>;
}
