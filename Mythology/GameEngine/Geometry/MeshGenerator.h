﻿#pragma once

namespace GameEngine
{
	template<typename MeshDataType>
	class MeshGenerator
	{
	public:
		using VertexType = typename MeshDataType::VertexType;

	public:
		static MeshDataType CreateRectangle(float originX, float originY, float width, float height, float depth)
		{
			MeshDataType meshData;

			meshData.Vertices =
			{
				{ { originX, originY - height, depth }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
				{ { originX, originY, depth }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
				{ { originX + width, originY, depth }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
				{ { originX + width, originY - height, depth }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } }
			};

			meshData.Indices =
			{
				0, 1, 2,
				0, 2, 3,
			};

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

			for (std::size_t i = 0; i < numberOfSubdivisions; ++i)
				meshData.Subdivide(meshData);

			return meshData;
		}
	};
}
