#include "pch.h"
#include "ShaderStage.h"

using namespace VulkanEngine;

VkPipelineShaderStageCreateInfo ShaderStage::Vertex(VkShaderModule shaderModule, const char* name)
{
	VkPipelineShaderStageCreateInfo shaderStageInfo = {};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStageInfo.module = shaderModule;
	shaderStageInfo.pName = name;
	return shaderStageInfo;
}

VkPipelineShaderStageCreateInfo ShaderStage::Fragment(VkShaderModule shaderModule, const char* name)
{
	VkPipelineShaderStageCreateInfo shaderStageInfo = {};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStageInfo.module = shaderModule;
	shaderStageInfo.pName = name;
	return shaderStageInfo;
}
