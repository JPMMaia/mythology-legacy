#pragma once

#include "VulkanEngine/Buffers/GeometryBuffer.h"
#include "VulkanEngine/Geometry/SubmeshGeometry.h"

namespace VulkanEngine
{
	class RenderItem
	{
	public:
		RenderItem(GeometryBuffer geometryBuffer, SubmeshGeometry submesh);

	public:
		void Draw(vk::Device device, vk::CommandBuffer commandBuffer);

	private:
		GeometryBuffer m_geometryBuffer;
		SubmeshGeometry m_submesh;
	};
}
