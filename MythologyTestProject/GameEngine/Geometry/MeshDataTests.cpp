#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/Geometry/MeshData.h"
#include "Libraries/tue/vec.hpp"

#include <DirectXMath.h>

using namespace GameEngine;
using namespace DirectX;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using XMMeshData = MeshData<XMFLOAT2, XMFLOAT3, uint32_t>;
using TueMeshData = MeshData<tue::fvec2, tue::fvec3, uint32_t>;

template <>
XMMeshData::VertexType XMMeshData::CalculateMidlleVertex(const VertexType& vertex0, const VertexType& vertex1)
{
	VertexType output;

	auto position = 0.5f * (XMLoadFloat3(&vertex0.Position) + XMLoadFloat3(&vertex1.Position));
	auto normal = XMVector3Normalize(0.5f * (XMLoadFloat3(&vertex0.Normal) + XMLoadFloat3(&vertex1.Normal)));
	auto tangent = XMVector3Normalize(0.5f * (XMLoadFloat3(&vertex0.Tangent) + XMLoadFloat3(&vertex1.Tangent)));
	auto textureCoordinates = 0.5f * (XMLoadFloat2(&vertex0.TextureCoordinates) + XMLoadFloat2(&vertex1.TextureCoordinates));

	XMStoreFloat3(&output.Position, position);
	XMStoreFloat3(&output.Normal, normal);
	XMStoreFloat3(&output.Tangent, tangent);
	XMStoreFloat2(&output.TextureCoordinates, textureCoordinates);

	return output;
}

template <>
TueMeshData::VertexType TueMeshData::CalculateMidlleVertex(const VertexType& vertex0, const VertexType& vertex1)
{
	VertexType output;

	output.Position = 0.5f * (vertex0.Position + vertex1.Position);
	output.Normal = tue::math::normalize(0.5f * (vertex0.Normal + vertex1.Normal));
	output.Tangent = tue::math::normalize(0.5f * (vertex0.Tangent + vertex1.Tangent));
	output.TextureCoordinates = 0.5f * (vertex0.TextureCoordinates + vertex1.TextureCoordinates);

	return output;
}

namespace MythologyTestProject
{
	TEST_CLASS(MeshDataTest)
	{
	public:

		TEST_METHOD(MeshDataTestXM)
		{
			XMMeshData meshData;
			XMMeshData::Subdivide(meshData);
		}

		TEST_METHOD(MeshDataTestTue)
		{
			TueMeshData meshData;
			TueMeshData::Subdivide(meshData);
		}
	};
}