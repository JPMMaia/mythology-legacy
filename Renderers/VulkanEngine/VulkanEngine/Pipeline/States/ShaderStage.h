#pragma once

#include <vulkan/vulkan.h>

#include <string>

namespace VulkanEngine
{
	class ShaderStage
	{
	public:
		static VkPipelineShaderStageCreateInfo Vertex(VkShaderModule shaderModule, const char* name);
		static VkPipelineShaderStageCreateInfo Fragment(VkShaderModule shaderModule, const char* name);
	};
}
