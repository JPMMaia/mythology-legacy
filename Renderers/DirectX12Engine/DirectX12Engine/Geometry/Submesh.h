#pragma once

#include "RenderEngine/Geometry/MeshData.h"

namespace DirectX12Engine
{
	class Submesh
	{
	public:
		Submesh(std::uint32_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0);

	public:
		std::uint32_t GetIndexCount() const;
		void SetIndexCount(std::uint32_t indexCount);

		std::uint32_t GetStartIndexLocation() const;
		void SetStartIndexLocation(std::uint32_t startIndexLocation);

		std::uint32_t GetBaseVertexLocation() const;
		void SetBaseVertexLocation(std::uint32_t baseVertexLocation);

	private:
		std::uint32_t m_indexCount;
		std::uint32_t m_startIndexLocation;
		std::uint32_t m_baseVertexLocation;
	};
}
