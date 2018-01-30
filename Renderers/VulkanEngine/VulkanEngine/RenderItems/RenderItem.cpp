#include "pch.h"
#include "RenderItem.h"

using namespace VulkanEngine;

RenderItem::RenderItem(GeometryBuffer geometryBuffer, SubmeshGeometry submesh) :
	m_geometryBuffer(std::move(geometryBuffer)),
	m_submesh(std::move(submesh))
{
}

void RenderItem::Draw(vk::Device device, vk::CommandBuffer commandBuffer)
{
	m_geometryBuffer.Bind(device, commandBuffer);

	commandBuffer.drawIndexed(
		m_submesh.IndexCount,
		m_submesh.InstanceCount,
		m_submesh.FirstIndex,
		m_submesh.VertexOffset,
		m_submesh.FirstInstance
	);
}
