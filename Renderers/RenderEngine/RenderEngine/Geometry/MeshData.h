#pragma once

#include <cstdint>
#include <vector>

namespace RenderEngine
{
	template<class VertexT, class IndexT>
	struct MeshData
	{
	public:
		using VertexType = VertexT;
		using IndexType = IndexT;

	public:
		std::vector<VertexType> Vertices;
		std::vector<IndexType> Indices;

	public:
		MeshData() = default;

		MeshData(std::vector<VertexType> vertices, std::vector<IndexType> indices) :
			Vertices(std::move(vertices)),
			Indices(std::move(indices))
		{
		}

		template <class InputIterator0, class InputIterator1>
		MeshData(InputIterator0 firstVertex, InputIterator0 lastVertex, InputIterator1 firstIndex, InputIterator1 lastIndex) :
			Vertices(firstVertex, lastVertex),
			Indices(firstIndex, lastIndex)
		{
		}

	public:
		std::size_t GetVertexDataByteSize() const
		{
			return Vertices.size() * sizeof(VertexType);
		}
		std::size_t GetIndexDataByteSize() const
		{
			return Indices.size() * sizeof(IndexType);
		}

	public:
		static MeshData Subdivide(const MeshData& meshData)
		{
			MeshData output;

			output.Vertices.reserve(meshData.Vertices.size() * 2);
			output.Indices.reserve(meshData.Indices.size() * 4);

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
					output.Vertices.emplace_back(std::move(vertex0));
					output.Vertices.emplace_back(std::move(vertex1));
					output.Vertices.emplace_back(std::move(vertex2));
					output.Vertices.emplace_back(std::move(middleVertex0));
					output.Vertices.emplace_back(std::move(middleVertex1));
					output.Vertices.emplace_back(std::move(middleVertex2));
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
		static VertexType CalculateMidlleVertex(const VertexType& vertex0, const VertexType& vertex1)
		{
			VertexType output;

			output.Position = 0.5f * (vertex0.Position + vertex1.Position);
			output.Normal = (vertex0.Normal + vertex1.Normal).normalized();
			output.Tangent = (vertex0.Tangent + vertex1.Tangent).normalized();
			output.TextureCoordinates = 0.5f * (vertex0.TextureCoordinates + vertex1.TextureCoordinates);

			return output;
		}
	};
}
