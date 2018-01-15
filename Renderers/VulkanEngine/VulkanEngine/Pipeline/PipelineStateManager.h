#pragma once

#include "VulkanEngine/Pipeline/Shader.h"

#include <string>
#include <unordered_map>

namespace VulkanEngine
{
	class PipelineStateManager
	{
	public:
		explicit PipelineStateManager(VkDevice device);

	private:
		void CreateShaders();
		void CreateShader(const std::string& name, const std::wstring& filename);

	private:
		VkDevice m_device;
		std::unordered_map<std::string, Shader> m_shaders;
	};
}
