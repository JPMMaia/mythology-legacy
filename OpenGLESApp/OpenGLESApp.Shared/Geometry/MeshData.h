#pragma once

#include "GameEngine/Geometry/MeshData.h"
#include "Libraries/tue/vec.hpp"
#include "GameEngine/Geometry/MeshGenerator.h"

using TueMeshData = GameEngine::MeshData<tue::fvec2, tue::fvec3, uint32_t>;
using TueMeshGenerator = GameEngine::MeshGenerator<TueMeshData>;

template <>
inline TueMeshData::VertexType TueMeshData::CalculateMidlleVertex(const VertexType& vertex0, const VertexType& vertex1)
{
	VertexType output;

	output.Position = 0.5f * (vertex0.Position + vertex1.Position);
	output.Normal = tue::math::normalize(0.5f * (vertex0.Normal + vertex1.Normal));
	output.Tangent = tue::math::normalize(0.5f * (vertex0.Tangent + vertex1.Tangent));
	output.TextureCoordinates = 0.5f * (vertex0.TextureCoordinates + vertex1.TextureCoordinates);

	return output;
}
