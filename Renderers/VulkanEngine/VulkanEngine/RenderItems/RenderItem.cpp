#include "pch.h"
#include "RenderItem.h"

using namespace VulkanEngine;

RenderItem::RenderItem(VertexBuffer vertexBuffer, IndexBuffer indexBuffer, const SubmeshGeometry& submesh) :
	m_vertexBuffer(std::move(vertexBuffer)),
	m_indexBuffer(std::move(indexBuffer)),
	m_submesh(submesh)
{
}

void RenderItem::Draw(vk::Device device, vk::CommandBuffer commandBuffer)
{
	m_vertexBuffer.Bind(device, commandBuffer);
	m_indexBuffer.Bind(device, commandBuffer);

	commandBuffer.drawIndexed(
		m_submesh.IndexCount,
		m_submesh.InstanceCount,
		m_submesh.FirstIndex,
		m_submesh.VertexOffset,
		m_submesh.FirstInstance
	);
}
