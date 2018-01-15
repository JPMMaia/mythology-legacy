#include "pch.h"
#include "Shader.h"
#include "Common/Helpers.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

#include <vector>

using namespace Common;
using namespace VulkanEngine;

Shader::Shader(VkDevice device, const std::wstring& filename) :
	m_device(device),
	m_shaderModule(CreateShaderModule(filename))
{
}
Shader::~Shader()
{
	vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
}

VkShaderModule Shader::GetShaderModule() const
{
	return m_shaderModule;
}

VkShaderModule Shader::CreateShaderModule(const std::wstring & filename)
{
	std::vector<char> shaderCode;
	Helpers::ReadData(filename, shaderCode);

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = shaderCode.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

	VkShaderModule shaderModule;
	ThrowIfFailed(vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule));

	return shaderModule;
}
