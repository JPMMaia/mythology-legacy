#include "pch.h"
#include "Shader.h"
#include "Common/Helpers.h"

using namespace Common;
using namespace VulkanEngine;

Shader::Shader(const vk::Device& device, const std::wstring& filename) :
	m_shaderModule(CreateShaderModule(device, filename))
{
}
Shader::operator const vk::ShaderModule&() const
{
	return m_shaderModule.get();
}

vk::UniqueShaderModule Shader::CreateShaderModule(const vk::Device& device, const std::wstring& filename)
{
	std::vector<char> shaderCode;
	Helpers::ReadData(filename, shaderCode);

	vk::ShaderModuleCreateInfo createInfo(
		{},
		shaderCode.size(),
		reinterpret_cast<const std::uint32_t*>(shaderCode.data())
	);

	return device.createShaderModuleUnique(createInfo);
}
