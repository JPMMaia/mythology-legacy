#pragma once

#include "VulkanEngine/Pipeline/Shader.h"
#include "VulkanEngine/Pipeline/DescriptorManager.h"
#include "VulkanEngine/RenderPass/RenderPass.h"

namespace VulkanEngine
{
	class PipelineStateManager
	{
	public:
		PipelineStateManager(vk::Device device, const DescriptorManager& descriptorManager, const RenderPass& renderPass);

	public:
		vk::PipelineLayout GetPipelineLayout() const;
		vk::Pipeline GetGraphicsPipeline() const;

	private:
		static vk::UniquePipelineLayout CreatePipelineLayout(vk::Device device, const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts);
		static vk::UniquePipeline CreateGraphicsPipeline(vk::Device device, const RenderPass& renderPass, vk::PipelineLayout pipelineLayout);
		static std::unordered_map<std::string, Shader> CreateShaders(vk::Device device);

	private:
		vk::UniquePipelineLayout m_pipelineLayout;
		vk::UniquePipeline m_graphicsPipeline;
	};
}
