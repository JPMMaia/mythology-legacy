#pragma once

#include "VertexData.h"

#include <vector>

namespace GameEngine
{
	template<typename TwoDType, typename ThreeDType, typename IndexType>
	struct MeshData
	{
	public:
		using VertexType = Vertex<TwoDType, ThreeDType>;

	public:
		std::vector<VertexType> Vertices;
		std::vector<IndexType> Indices;

	public:
		static MeshData Subdivide(const MeshData& meshData)
		{
			MeshData output;

			auto numTriangles = static_cast<std::size_t>(meshData.Indices.size()) / 3;
			for (std::size_t i = 0; i < numTriangles; ++i)
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
					auto triangleIndexOffset = static_cast<IndexType>(i * 6);
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

	private:
		static VertexType CalculateMidlleVertex(const VertexType& vertex0, const VertexType& vertex1);
	};
}
