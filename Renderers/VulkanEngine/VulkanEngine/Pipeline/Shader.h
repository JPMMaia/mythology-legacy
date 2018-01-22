#pragma once

namespace VulkanEngine
{
	class Shader
	{
	public:
		Shader(const vk::Device& device, const std::wstring& filename);

	public:
		operator const vk::ShaderModule&() const;

	private:
		static vk::UniqueShaderModule CreateShaderModule(const vk::Device& device, const std::wstring& filename);

	private:
		vk::UniqueShaderModule m_shaderModule;
	};
}
