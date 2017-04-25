#pragma once

#include <DirectXCollision.h>
#include "MeshGenerator.h"

namespace DirectX12Engine
{
	class Submesh
	{
	public:
		template<typename VertexType>
		static DirectX::BoundingBox CreateBoundingBoxFromMesh(const std::vector<VertexType>& vertices)
		{
			using namespace DirectX;

			auto infinity = std::numeric_limits<float>::infinity();

			auto positionMin = XMVectorSet(+infinity, +infinity, +infinity, 0.0f);
			auto positionMax = XMVectorSet(-infinity, -infinity, -infinity, 0.0f);
			for (auto& vertex : vertices)
			{
				auto position = XMLoadFloat3(&vertex.Position);
				positionMin = XMVectorMin(positionMin, position);
				positionMax = XMVectorMax(positionMax, position);
			}

			BoundingBox bounds;
			XMStoreFloat3(&bounds.Center, 0.5f * (positionMin + positionMax));
			XMStoreFloat3(&bounds.Extents, 0.5f * (positionMax - positionMin));

			return bounds;
		}

	public:
		explicit Submesh(const MeshGenerator::MeshData& meshData);
		Submesh(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation, const DirectX::BoundingBox& bounds);

		uint32_t IndexCount() const;
		void SetIndexCount(uint32_t indexCount);

		uint32_t StartIndexLocation() const;
		void SetStartIndexLocation(uint32_t startIndexLocation);

		uint32_t BaseVertexLocation() const;
		void SetBaseVertexLocation(uint32_t baseVertexLocation);

		DirectX::BoundingBox Bounds() const;
		void SetBounds(const DirectX::BoundingBox& bounds);

	private:
		uint32_t m_indexCount;
		uint32_t m_startIndexLocation;
		uint32_t m_baseVertexLocation;
		DirectX::BoundingBox m_bounds;
	};
}
