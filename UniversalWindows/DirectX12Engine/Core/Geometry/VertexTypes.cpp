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

std::vector<VertexTypes::PositionNormalTextureCoordinatesSkinnedVertex> VertexTypes::PositionNormalTextureCoordinatesSkinnedVertex::CreateFromMeshData(const GameEngine::EigenMeshData& meshData)
{
	using VertexType = PositionNormalTextureCoordinatesSkinnedVertex;
	
	std::vector<VertexType> vertices;
	vertices.reserve(meshData.Vertices.size());

	for (const auto& meshVertex : meshData.Vertices)
	{
		VertexType vertex;
		vertex.PositionL = meshVertex.Position;
		vertex.NormalL = meshVertex.Normal;
		vertex.TextureCoordinates = meshVertex.TextureCoordinates;
		
		std::copy_n(meshVertex.BoneWeights.begin(), (std::min)(meshVertex.BoneWeights.size(), vertex.Weights.size()), vertex.Weights.begin());
		std::copy_n(meshVertex.BoneIndices.begin(), (std::min)(meshVertex.BoneIndices.size(), vertex.BoneIndices.size()), vertex.BoneIndices.begin());

		vertices.push_back(std::move(vertex));
	}
		

	return vertices;
}