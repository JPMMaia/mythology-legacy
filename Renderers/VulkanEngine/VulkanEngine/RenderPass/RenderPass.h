#pragma once

#include <vulkan/vulkan.hpp>

namespace VulkanEngine
{
	class RenderPass
	{
	public:
		explicit RenderPass(const vk::Device& device, vk::Format format);
		RenderPass(const RenderPass&) = delete;
		RenderPass(RenderPass&&) = default;

	public:
		RenderPass& operator=(const RenderPass&) = delete;
		RenderPass& operator=(RenderPass&&) = default;

	public:
		operator const vk::RenderPass&() const;

	private:
		static vk::UniqueRenderPass CreateRenderPass(const vk::Device& device, vk::Format format);

	private:
		vk::UniqueRenderPass m_renderPass;
	};
}
