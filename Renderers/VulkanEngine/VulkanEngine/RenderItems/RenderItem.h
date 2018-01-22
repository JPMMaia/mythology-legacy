#pragma once

#include "VulkanEngine/Buffers/VertexBuffer.h"
#include "VulkanEngine/Geometry/SubmeshGeometry.h"

namespace VulkanEngine
{
	class RenderItem
	{
	public:
		explicit RenderItem(VertexBuffer&& vertexBuffer, const SubmeshGeometry& submesh);

	public:
		void Draw(const vk::CommandBuffer& commandBuffer);

	private:
		VertexBuffer m_vertexBuffer;
		SubmeshGeometry m_submesh;
	};
}
