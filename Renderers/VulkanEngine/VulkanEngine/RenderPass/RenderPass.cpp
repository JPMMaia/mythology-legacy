#include "pch.h"
#include "RenderPass.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanEngine;

RenderPass::RenderPass(const vk::Device& device, vk::Format format) :
	m_renderPass(CreateRenderPass(device, format))
{
}

RenderPass::operator const vk::RenderPass&() const
{
	return m_renderPass.get();
}

vk::UniqueRenderPass RenderPass::CreateRenderPass(const vk::Device& device, vk::Format format)
{
	vk::AttachmentDescription colorAttachment(
		{},
		format,
		vk::SampleCountFlagBits::e1,
		vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eStore,
		vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::ePresentSrcKHR
	);

	vk::AttachmentReference colorAttachmentReference(
		0,
		vk::ImageLayout::eColorAttachmentOptimal
	);

	vk::SubpassDescription subpass(
		{},
		vk::PipelineBindPoint::eGraphics,
		0, nullptr,
		1, &colorAttachmentReference
	);

	vk::SubpassDependency dependency(
		VK_SUBPASS_EXTERNAL, 0,
		vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput,
		{}, vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite,
		{}
	);

	vk::RenderPassCreateInfo renderPassInfo(
		{},
		1, &colorAttachment,
		1, &subpass,
		1, &dependency
	);

	return device.createRenderPassUnique(renderPassInfo);
}
