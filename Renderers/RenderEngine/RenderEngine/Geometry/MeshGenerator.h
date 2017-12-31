#pragma once

#include "MeshData.h"

namespace RenderEngine
{
	class MeshGenerator
	{
	public:
		using VertexType = Vertex;
		using MeshDataType = MeshData;

	public:
		static MeshDataType CreateRectangle(float originX, float originY, float width, float height, float depth, std::size_t numberOfSubdivisions)
		{
			MeshDataType meshData;

			auto halfWidth = width / 2.0f;
			auto halfHeight = height / 2.0f;

			meshData.Vertices =
			{
				{ { originX + halfWidth, originY - halfHeight, depth },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
			{ { originX + halfWidth, originY + halfHeight, depth },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
			{ { originX - halfWidth, originY + halfHeight, depth },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
			{ { originX - halfWidth, originY - halfHeight, depth },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
			};

			meshData.Indices =
			{
				0, 1, 2,
				0, 2, 3,
			};

			for (std::size_t i = 0; i < numberOfSubdivisions; ++i)
				meshData.Subdivide(meshData);

			return meshData;
		}
		static MeshDataType CreateBox(float width, float height, float depth, std::size_t numberOfSubdivisions)
		{
			MeshDataType meshData;

			auto halfWidth = 0.5f * width;
			auto halfHeight = 0.5f * height;
			auto halfDepth = 0.5f * depth;

			meshData.Vertices =
			{
				// Front face:
				{ { +halfWidth, -halfHeight, +halfDepth },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
			{ { +halfWidth, +halfHeight, +halfDepth },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
			{ { -halfWidth, +halfHeight, +halfDepth },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
			{ { -halfWidth, -halfHeight, +halfDepth },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },

			// Back face:
			{ { -halfWidth, +halfHeight, -halfDepth },{ 0.0f, 0.0f, -1.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
			{ { +halfWidth, +halfHeight, -halfDepth },{ 0.0f, 0.0f, -1.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
			{ { +halfWidth, -halfHeight, -halfDepth },{ 0.0f, 0.0f, -1.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
			{ { -halfWidth, -halfHeight, -halfDepth },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },

			// Top face:
			{ { +halfWidth, +halfHeight, +halfDepth },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
			{ { +halfWidth, +halfHeight, -halfDepth },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
			{ { -halfWidth, +halfHeight, -halfDepth },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
			{ { -halfWidth, +halfHeight, +halfDepth },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },

			// Bottom face:
			{ { -halfWidth, -halfHeight, -halfDepth },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
			{ { +halfWidth, -halfHeight, -halfDepth },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
			{ { +halfWidth, -halfHeight, +halfDepth },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
			{ { -halfWidth, -halfHeight, +halfDepth },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },

			// Left face:
			{ { -halfWidth, -halfHeight, +halfDepth },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 1.0f } },
			{ { -halfWidth, +halfHeight, +halfDepth },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f } },
			{ { -halfWidth, +halfHeight, -halfDepth },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 0.0f } },
			{ { -halfWidth, -halfHeight, -halfDepth },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 0.0f, 1.0f } },

			// Right face:
			{ { +halfWidth, -halfHeight, -halfDepth },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 1.0f } },
			{ { +halfWidth, +halfHeight, -halfDepth },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f } },
			{ { +halfWidth, +halfHeight, +halfDepth },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f } },
			{ { +halfWidth, -halfHeight, +halfDepth },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },

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

			for (std::size_t i = 0; i < numberOfSubdivisions; ++i)
				meshData.Subdivide(meshData);

			return meshData;
		}
	};
}
