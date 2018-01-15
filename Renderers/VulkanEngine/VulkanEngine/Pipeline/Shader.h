#pragma once

#include <vulkan/vulkan.h>

#include <string>

namespace VulkanEngine
{
	class Shader
	{
	public:
		Shader(VkDevice device, const std::wstring& filename);
		~Shader();

		VkShaderModule GetShaderModule() const;

	private:
		VkShaderModule CreateShaderModule(const std::wstring& filename);

	private:
		VkDevice m_device;
		VkShaderModule m_shaderModule;
	};
}
