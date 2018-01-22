#pragma once

#include <vulkan/vulkan.hpp>

namespace VulkanEngine
{
	class PipelineLayout
	{
	public:
		explicit PipelineLayout(const vk::Device& device);

	public:
		operator const vk::PipelineLayout&() const;

	private:
		static vk::UniquePipelineLayout CreatePipelineLayout(const vk::Device& device);

	private:
		vk::UniquePipelineLayout m_pipelineLayout;
	};
}
