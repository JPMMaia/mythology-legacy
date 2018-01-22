#pragma once

namespace VulkanEngine
{
	class ShaderStage
	{
	public:
		static vk::PipelineShaderStageCreateInfo Vertex(const vk::ShaderModule& shaderModule, const char* name);
		static vk::PipelineShaderStageCreateInfo Fragment(const vk::ShaderModule& shaderModule, const char* name);
	};
}
