#pragma once

#include "VulkanEngine/Pipeline/Shader.h"
#include "VulkanEngine/Pipeline/Layout/PipelineLayout.h"
#include "VulkanEngine/Pipeline/RenderPass/RenderPass.h"

namespace VulkanEngine
{
	class PipelineStateManager
	{
	public:
		explicit PipelineStateManager(const vk::Device& device, vk::Format format, float width, float height, const vk::Extent2D& extent);

	public:
		const RenderPass& GetRenderPass() const;
		const vk::Pipeline& GetGraphicsPipeline() const;

	private:
		static std::unordered_map<std::string, Shader> CreateShaders(const vk::Device& device);
		static vk::UniquePipeline CreateGraphicsPipeline(const vk::Device& device, const vk::Viewport& viewport, const vk::Rect2D& scissor, const RenderPass& renderPass, const PipelineLayout& pipelineLayout, float width, float height, const vk::Extent2D& extent);

	private:
		vk::Viewport m_viewport;
		vk::Rect2D m_scissor;
		RenderPass m_renderPass;
		PipelineLayout m_pipelineLayout;
		vk::UniquePipeline m_graphicsPipeline;
	};
}
