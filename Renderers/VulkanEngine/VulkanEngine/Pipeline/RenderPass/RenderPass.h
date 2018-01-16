#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class RenderPass
	{
	public:
		explicit RenderPass(VkDevice device, VkFormat format);
		~RenderPass();

	public:
		operator VkRenderPass() const;

	private:
		static VkRenderPass CreateRenderPass(VkDevice device, VkFormat format);

	private:
		VkDevice m_device;
		VkRenderPass m_renderPass;
	};
}
