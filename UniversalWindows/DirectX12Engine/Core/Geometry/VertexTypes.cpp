#include "pch.h"
#include "VertexTypes.h"

using namespace DirectX12Engine;

std::vector<VertexTypes::PositionVertex> VertexTypes::PositionVertex::CreateFromMeshData(const MeshGenerator::MeshData& meshData)
{
	std::vector<PositionVertex> vertices;
	vertices.reserve(meshData.Vertices.size());

	for(const auto& vertex : meshData.Vertices)
		vertices.push_back({ vertex.Position });

	return vertices;
}
std::vector<VertexTypes::PositionTextureCoordinatesVextex> VertexTypes::PositionTextureCoordinatesVextex::CreateFromMeshData(const MeshGenerator::MeshData& meshData)
{
	std::vector<PositionTextureCoordinatesVextex> vertices;
	vertices.reserve(meshData.Vertices.size());

	for (const auto& vertex : meshData.Vertices)
		vertices.push_back({ vertex.Position, vertex.TextureCoordinates });

	return vertices;
}
