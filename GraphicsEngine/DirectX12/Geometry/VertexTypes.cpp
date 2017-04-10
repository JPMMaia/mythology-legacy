#include "pch.h"
#include "VertexTypes.h"

using namespace GraphicsEngine;

std::vector<VertexTypes::PositionVertex> VertexTypes::PositionVertex::CreateFromMeshData(const MeshGenerator::MeshData& meshData)
{
	std::vector<PositionVertex> vertices;
	vertices.reserve(meshData.Vertices.size());

	for(const auto& vertex : meshData.Vertices)
		vertices.push_back({ vertex.Position });

	return vertices;
}
