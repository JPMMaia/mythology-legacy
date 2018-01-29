#include "pch.h"
#include "RenderItem.h"

using namespace VulkanEngine;

RenderItem::RenderItem(VertexBuffer&& vertexBuffer, const SubmeshGeometry& submesh) :
	m_vertexBuffer(std::move(vertexBuffer)),
	m_submesh(submesh)
{
}

void RenderItem::Draw(vk::Device device, vk::CommandBuffer commandBuffer)
{
	m_vertexBuffer.Bind(device, commandBuffer);

	commandBuffer.draw(
		m_submesh.VertexCount, 
		m_submesh.InstanceCount, 
		m_submesh.FirstVertex,
		m_submesh.FirstInstance
	);
}
