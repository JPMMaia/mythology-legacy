#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class RenderPass
	{
	public:
		explicit RenderPass(VkDevice device, VkFormat format);
		RenderPass(const RenderPass&) = delete;
		RenderPass(RenderPass&&) = default;
		~RenderPass();

		RenderPass& operator=(const RenderPass&) = delete;
		RenderPass& operator=(RenderPass&&) = default;

	public:
		operator VkRenderPass() const;

	private:
		static VkRenderPass CreateRenderPass(VkDevice device, VkFormat format);

	private:
		VkDevice m_device;
		VkRenderPass m_renderPass;
	};
}
