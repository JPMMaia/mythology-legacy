#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace VulkanEngine
{
	class VulkanInstance
	{
	public:
		VulkanInstance() = default;
		explicit VulkanInstance(const std::vector<const char*>& enabledExtensions);
		~VulkanInstance();

	public:
		operator VkInstance() const;

	public:
		std::vector<const char*> GetValidationLayers() const;

	private:
		bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);

	private:
		VkInstance m_instance;
	};
}