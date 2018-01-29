#pragma once

#include "VulkanEngine/Buffers/VertexBuffer.h"
#include "VulkanEngine/Buffers/IndexBuffer.h"
#include "VulkanEngine/Geometry/SubmeshGeometry.h"

namespace VulkanEngine
{
	class RenderItem
	{
	public:
		RenderItem(VertexBuffer vertexBuffer, IndexBuffer indexBuffer, const SubmeshGeometry& submesh);

	public:
		void Draw(vk::Device device, vk::CommandBuffer commandBuffer);

	private:
		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;
		SubmeshGeometry m_submesh;
	};
}
