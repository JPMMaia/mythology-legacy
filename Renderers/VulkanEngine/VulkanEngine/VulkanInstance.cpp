#include "pch.h"
#include "VulkanInstance.h"
#include "VulkanEngine/Configurations/ValidationLayers.h"

#include <cstring>

using namespace VulkanEngine;

VulkanInstance::VulkanInstance(const std::vector<const char*>& enabledExtensions) :
	m_instance(CreateInstance(enabledExtensions))
{
}

VulkanInstance::operator const vk::Instance&() const
{
	return m_instance.get();
}

vk::UniqueInstance VulkanInstance::CreateInstance(const std::vector<const char*>& enabledExtensions)
{
	vk::ApplicationInfo applicationInfo(
		"Vulkan Application",
		VK_MAKE_VERSION(1, 0, 0),
		"Vulkan Engine",
		VK_MAKE_VERSION(1, 0, 0),
		VK_API_VERSION_1_0
		);

	auto validationLayers = ValidationLayers::Get();
	vk::InstanceCreateInfo createInfo(
		{},
		&applicationInfo,
		static_cast<std::uint32_t>(validationLayers.size()),
		validationLayers.data(),
		static_cast<std::uint32_t>(enabledExtensions.size()),
		enabledExtensions.data()
	);

	return vk::createInstanceUnique(createInfo);
}
bool VulkanInstance::CheckValidationLayerSupport(const std::vector<const char*>& validationLayers)
{
	std::uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (auto layerName : validationLayers)
	{
		auto layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (std::strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
			return false;
	}

	return true;
}
