#include "pch.h"
#include "MeshGenerator.h"

#include <array>

using namespace DirectX;
using namespace GraphicsEngine;

MeshGenerator::Vertex::Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT3& tangentU, const DirectX::XMFLOAT2& textureCoordinates) :
	Position(position),
	Normal(normal),
	Tangent(tangentU),
	TextureCoordinates(textureCoordinates)
{
}

MeshGenerator::MeshData MeshGenerator::MeshData::Subdivide(const MeshData& meshData)
{
	MeshData output;

	uint32_t numTriangles = static_cast<uint32_t>(meshData.Indices.size()) / 3;
	for (uint32_t i = 0; i < numTriangles; ++i)
	{
		// Vertices:
		{
			// Get vertices of triangle:
			auto triangleIndexOffset = i * 3;
			auto vertex0 = meshData.Vertices[meshData.Indices[triangleIndexOffset + 0]];
			auto vertex1 = meshData.Vertices[meshData.Indices[triangleIndexOffset + 1]];
			auto vertex2 = meshData.Vertices[meshData.Indices[triangleIndexOffset + 2]];

			// Calculate middle vertices:
			auto middleVertex0 = CalculateMidlleVertex(vertex0, vertex1);
			auto middleVertex1 = CalculateMidlleVertex(vertex1, vertex2);
			auto middleVertex2 = CalculateMidlleVertex(vertex2, vertex0);

			// Add vertices:
			output.Vertices.push_back(vertex0);
			output.Vertices.push_back(vertex1);
			output.Vertices.push_back(vertex2);
			output.Vertices.push_back(middleVertex0);
			output.Vertices.push_back(middleVertex1);
			output.Vertices.push_back(middleVertex2);
		}

		// Indices:
		{
			auto triangleIndexOffset = i * 6;
			output.Indices.push_back(triangleIndexOffset + 0);
			output.Indices.push_back(triangleIndexOffset + 3);
			output.Indices.push_back(triangleIndexOffset + 5);

			output.Indices.push_back(triangleIndexOffset + 3);
			output.Indices.push_back(triangleIndexOffset + 4);
			output.Indices.push_back(triangleIndexOffset + 5);

			output.Indices.push_back(triangleIndexOffset + 5);
			output.Indices.push_back(triangleIndexOffset + 4);
			output.Indices.push_back(triangleIndexOffset + 2);

			output.Indices.push_back(triangleIndexOffset + 3);
			output.Indices.push_back(triangleIndexOffset + 1);
			output.Indices.push_back(triangleIndexOffset + 4);
		}
	}

	return output;
}

MeshGenerator::Vertex MeshGenerator::MeshData::CalculateMidlleVertex(const Vertex& vertex0, const Vertex& vertex1)
{
	Vertex output;

	auto position = 0.5f * (XMLoadFloat3(&vertex0.Position) + XMLoadFloat3(&vertex1.Position));
	auto normal = XMVector3Normalize(0.5f * (XMLoadFloat3(&vertex0.Normal) + XMLoadFloat3(&vertex1.Normal)));
	auto tangent = XMVector3Normalize(0.5f * (XMLoadFloat3(&vertex0.Tangent) + XMLoadFloat3(&vertex1.Tangent)));
	auto textureCoordinates = 0.5f * (XMLoadFloat3(&vertex0.Position) + XMLoadFloat3(&vertex1.Position));

	XMStoreFloat3(&output.Position, position);
	XMStoreFloat3(&output.Normal, normal);
	XMStoreFloat3(&output.Tangent, tangent);
	XMStoreFloat2(&output.TextureCoordinates, textureCoordinates);

	return output;
}

MeshGenerator::MeshData MeshGenerator::CreateRectangle(float originX, float originY, float width, float height, float depth)
{
	MeshData meshData;

	meshData.Vertices =
	{
		{ XMFLOAT3(originX, originY - height, depth), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(originX, originY, depth), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(originX + width, originY, depth), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(originX + width, originY - height, depth), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }
	};

	meshData.Indices =
	{
		0, 1, 2,
		0, 2, 3,
	};

	return meshData;
}
MeshGenerator::MeshData MeshGenerator::CreateBox(float width, float height, float depth, uint32_t numSubdivisions)
{
	MeshData meshData;

	auto halfWidth = 0.5f * width;
	auto halfHeight = 0.5f * height;
	auto halfDepth = 0.5f * depth;

	meshData.Vertices =
	{
		// Front face:
		{ { -halfWidth, -halfHeight, -halfDepth },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
		{ { -halfWidth, +halfHeight, -halfDepth },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
		{ { +halfWidth, +halfHeight, -halfDepth },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
		{ { +halfWidth, -halfHeight, -halfDepth },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },

		// Back face:
		{ { -halfWidth, -halfHeight, +halfDepth },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
		{ { +halfWidth, -halfHeight, +halfDepth },{ 0.0f, 0.0f, 1.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
		{ { +halfWidth, +halfHeight, +halfDepth },{ 0.0f, 0.0f, 1.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
		{ { -halfWidth, +halfHeight, +halfDepth },{ 0.0f, 0.0f, 1.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },

		// Top face:
		{ { -halfWidth, +halfHeight, -halfDepth },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
		{ { -halfWidth, +halfHeight, +halfDepth },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
		{ { +halfWidth, +halfHeight, +halfDepth },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
		{ { +halfWidth, +halfHeight, -halfDepth },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },

		// Bottom face:
		{ { -halfWidth, -halfHeight, -halfDepth },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
		{ { +halfWidth, -halfHeight, -halfDepth },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
		{ { +halfWidth, -halfHeight, +halfDepth },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
		{ { -halfWidth, -halfHeight, +halfDepth },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },

		// Left face:
		{ { -halfWidth, -halfHeight, +halfDepth },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 1.0f } },
		{ { -halfWidth, +halfHeight, +halfDepth },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f } },
		{ { -halfWidth, +halfHeight, -halfDepth },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f } },
		{ { -halfWidth, -halfHeight, -halfDepth },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 1.0f } },

		// Right face:
		{ { +halfWidth, -halfHeight, -halfDepth },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },
		{ { +halfWidth, +halfHeight, -halfDepth },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f } },
		{ { +halfWidth, +halfHeight, +halfDepth },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f } },
		{ { +halfWidth, -halfHeight, +halfDepth },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 1.0f } },

	};

	meshData.Indices =
	{
		// Front face:
		0, 1, 2,
		0, 2, 3,

		// Back face:
		4, 5, 6,
		4, 6, 7,

		// Top face:
		8, 9, 10,
		8, 10, 11,

		// Bottom face:
		12, 13, 14,
		12, 14, 15,

		// Left face:
		16, 17, 18,
		16, 18, 19,

		// Right face:
		20, 21, 22,
		20, 22, 23,
	};

	for (uint32_t i = 0; i < numSubdivisions; ++i)
		meshData.Subdivide(meshData);

	return meshData;
}

void MeshGenerator::CalculateNormalAndTangentVectors(MeshData& meshData, uint32_t verticesPerFace)
{
	const auto& vertices = meshData.Vertices;
	const auto& indices = meshData.Indices;

	for (uint32_t i = 0; i < meshData.Indices.size(); i += verticesPerFace)
	{
		auto vertex0 = vertices[indices[i]];
		auto vertex1 = vertices[indices[i + 1]];
		auto vertex2 = vertices[indices[i + 2]];

		XMFLOAT3 tangent, bitangent, normal;
		CalculateTangentBitangentNormal(vertex0.Position, vertex1.Position, vertex2.Position, vertex0.TextureCoordinates, vertex1.TextureCoordinates, vertex2.TextureCoordinates, tangent, bitangent, normal);

		// TODO
	}
}

void MeshGenerator::CalculateTangentBitangentNormal(const XMFLOAT3& position0, const XMFLOAT3& position1, const XMFLOAT3& position2, const XMFLOAT2& textureCoordinates0, const XMFLOAT2 textureCoordidanates1, const XMFLOAT2 textureCoordinates2, XMFLOAT3& tangent, XMFLOAT3& bitangent, XMFLOAT3& normal)
{
	// [ deltaU0 deltaV0 ] [ Tx Ty Tz ]     [ edge0.x edge0.y edge0.z ]
	// [                 ] [          ]  =  [                         ]
	// [ deltaU1 deltaV1 ] [ Bx By Bz ]     [ edge1.x edge1.y edge1.z ]

	// Set texture edge matrix:
	auto deltaUVMatrix = XMMatrixSet(
		textureCoordidanates1.x - textureCoordinates0.x, textureCoordidanates1.y - textureCoordinates0.y, 0.0f, 0.0f,
		textureCoordinates2.x - textureCoordinates0.x, textureCoordinates2.y - textureCoordinates0.y, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// Calculate inverse of texture edge matrix:
	auto determinant = XMMatrixDeterminant(deltaUVMatrix);
	auto deltaUVInverseMatrix = XMMatrixInverse(&determinant, deltaUVMatrix);

	// Set edge matrix:
	auto edgeMatrix = XMMatrixSet(
		position1.x - position0.x, position1.y - position0.y, position1.z - position0.z, 0.0f,
		position2.x - position0.x, position2.y - position0.y, position2.z - position0.z, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// Calculate the tangent and binormal matrix:
	XMFLOAT3X3 tangentBinormalMatrix;
	XMStoreFloat3x3(&tangentBinormalMatrix, XMMatrixMultiply(deltaUVInverseMatrix, edgeMatrix));

	auto tangentVector = XMVector3Normalize(XMVectorSet(tangentBinormalMatrix(0, 0), tangentBinormalMatrix(0, 1), tangentBinormalMatrix(0, 2), 0.0f));
	auto bitangentVector = XMVector3Normalize(XMVectorSet(tangentBinormalMatrix(1, 0), tangentBinormalMatrix(1, 1), tangentBinormalMatrix(1, 2), 0.0f));
	auto normalVector = XMVector3Cross(tangentVector, bitangentVector);

	XMStoreFloat3(&tangent, tangentVector);
	XMStoreFloat3(&bitangent, bitangentVector);
	XMStoreFloat3(&normal, normalVector);
}