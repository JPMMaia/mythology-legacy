#include "pch.h"
#include "ShaderStage.h"

using namespace VulkanEngine;

vk::PipelineShaderStageCreateInfo ShaderStage::Vertex(const vk::ShaderModule& shaderModule, const char* name)
{
	return vk::PipelineShaderStageCreateInfo(
		{},
		vk::ShaderStageFlagBits::eVertex,
		shaderModule,
		name
	);
}

vk::PipelineShaderStageCreateInfo ShaderStage::Fragment(const vk::ShaderModule& shaderModule, const char* name)
{
	return vk::PipelineShaderStageCreateInfo(
		{},
		vk::ShaderStageFlagBits::eFragment,
		shaderModule,
		name
	);
}
