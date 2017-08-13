#include "pch.h"
#include "VertexTypes.h"

using namespace DirectX12Engine;

std::vector<VertexTypes::PositionVertex> VertexTypes::PositionVertex::CreateFromMeshData(const GameEngine::EigenMeshData& meshData)
{
	std::vector<PositionVertex> vertices;
	vertices.reserve(meshData.Vertices.size());

	for(const auto& vertex : meshData.Vertices)
		vertices.push_back({ vertex.Position });

	return vertices;
}

std::vector<VertexTypes::PositionNormalVertex> VertexTypes::PositionNormalVertex::CreateFromMeshData(const GameEngine::EigenMeshData& meshData)
{
	std::vector<PositionNormalVertex> vertices;
	vertices.reserve(meshData.Vertices.size());

	for (const auto& vertex : meshData.Vertices)
		vertices.push_back({ vertex.Position, vertex.Normal });

	return vertices;
}

std::vector<VertexTypes::PositionNormalTextureCoordinatesVertex> VertexTypes::PositionNormalTextureCoordinatesVertex::CreateFromMeshData(const GameEngine::EigenMeshData& meshData)
{
	std::vector<PositionNormalTextureCoordinatesVertex> vertices;
	vertices.reserve(meshData.Vertices.size());

	for (const auto& vertex : meshData.Vertices)
		vertices.push_back({ vertex.Position, vertex.Normal, vertex.TextureCoordinates });

	return vertices;
}

std::vector<VertexTypes::PositionTextureCoordinatesVextex> VertexTypes::PositionTextureCoordinatesVextex::CreateFromMeshData(const GameEngine::EigenMeshData& meshData)
{
	std::vector<PositionTextureCoordinatesVextex> vertices;
	vertices.reserve(meshData.Vertices.size());

	for (const auto& vertex : meshData.Vertices)
		vertices.push_back({ vertex.Position, vertex.TextureCoordinates });

	return vertices;
}
