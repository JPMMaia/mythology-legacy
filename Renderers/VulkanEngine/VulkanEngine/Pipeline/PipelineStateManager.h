#pragma once

#include "VulkanEngine/Pipeline/Shader.h"
#include "VulkanEngine/Pipeline/Layout/PipelineLayout.h"
#include "VulkanEngine/RenderPass/RenderPass.h"

namespace VulkanEngine
{
	class PipelineStateManager
	{
	public:
		PipelineStateManager(const vk::Device& device, const RenderPass& renderPass);

	public:
		const vk::Pipeline& GetGraphicsPipeline() const;

	private:
		static std::unordered_map<std::string, Shader> CreateShaders(const vk::Device& device);
		static vk::UniquePipeline CreateGraphicsPipeline(const vk::Device& device, const RenderPass& renderPass, const PipelineLayout& pipelineLayout);

	private:
		PipelineLayout m_pipelineLayout;
		vk::UniquePipeline m_graphicsPipeline;
	};
}
