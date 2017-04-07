#pragma once

#include <DirectXCollision.h>

namespace GraphicsEngine
{
	class SubMesh
	{
	public:
		SubMesh(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation, const DirectX::BoundingBox& bounds);

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
