#pragma once

#include "VulkanEngine/Pipeline/Shader.h"
#include "VulkanEngine/Pipeline/Layout/PipelineLayout.h"
#include "VulkanEngine/Pipeline/RenderPass/RenderPass.h"

#include <string>
#include <unordered_map>

namespace VulkanEngine
{
	class PipelineStateManager
	{
	private:
		using ShaderContainer = std::unordered_map<std::string, Shader>;

	public:
		explicit PipelineStateManager(VkDevice device, VkFormat format, float width, float height, VkExtent2D extent);
		~PipelineStateManager();

	public:
		const RenderPass& GetRenderPass() const;
		VkPipeline GetGraphicsPipeline() const;

	private:
		static std::unordered_map<std::string, Shader> CreateShaders(VkDevice device);
		static VkPipeline CreateGraphicsPipeline(VkDevice device, const RenderPass& renderPass, const PipelineLayout& pipelineLayout, const ShaderContainer& shaders, float width, float height, VkExtent2D extent);

	private:
		VkDevice m_device;
		RenderPass m_renderPass;
		PipelineLayout m_pipelineLayout;
		ShaderContainer m_shaders;
		VkPipeline m_graphicsPipeline;
	};
}
