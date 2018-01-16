#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class PipelineLayout
	{
	public:
		explicit PipelineLayout(VkDevice device);
		~PipelineLayout();

	public:
		operator VkPipelineLayout() const;

	private:
		static VkPipelineLayout CreatePipelineLayout(VkDevice device);

	private:
		VkDevice m_device;
		VkPipelineLayout m_pipelineLayout;
	};
}
