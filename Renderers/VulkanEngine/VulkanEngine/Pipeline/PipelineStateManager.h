#pragma once

#include "VulkanEngine/Pipeline/Shader.h"
#include "VulkanEngine/Pipeline/Layout/PipelineLayout.h"
#include "VulkanEngine/Pipeline/RenderPass/RenderPass.h"

namespace VulkanEngine
{
	class PipelineStateManager
	{
	private:
		using ShaderContainer = std::unordered_map<std::string, Shader>;

	public:
		explicit PipelineStateManager(const vk::Device& device, vk::Format format, float width, float height, const vk::Extent2D& extent);

	public:
		const RenderPass& GetRenderPass() const;
		const vk::Pipeline& GetGraphicsPipeline() const;

	private:
		static std::unordered_map<std::string, Shader> CreateShaders(const vk::Device& device);
		static vk::UniquePipeline CreateGraphicsPipeline(const vk::Device& device, const RenderPass& renderPass, const PipelineLayout& pipelineLayout, const ShaderContainer& shaders, float width, float height, const vk::Extent2D& extent);

	private:
		RenderPass m_renderPass;
		PipelineLayout m_pipelineLayout;
		ShaderContainer m_shaders;
		vk::UniquePipeline m_graphicsPipeline;
	};
}
