#include "pch.h"
#include "Instance.h"
#include "VulkanEngine/Configurations/ValidationLayers.h"

#include <cstring>

using namespace VulkanEngine;

Instance::Instance(const std::vector<const char*>& enabledExtensions) :
	m_instance(CreateInstance(enabledExtensions))
{
}

Instance::operator const vk::Instance&() const
{
	return m_instance.get();
}

vk::UniqueInstance Instance::CreateInstance(const std::vector<const char*>& enabledExtensions)
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
bool Instance::CheckValidationLayerSupport(const std::vector<const char*>& validationLayers)
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
