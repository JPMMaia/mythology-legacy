#include "pch.h"
#include "VulkanInstance.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

#include <cstring>
#include <vector>

using namespace VulkanEngine;

VulkanInstance::VulkanInstance(const std::vector<const char*>& enabledExtensions)
{
	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = "Vulkan Application";
	applicationInfo.applicationVersion = VK_API_VERSION_1_0;
	applicationInfo.pEngineName = "No Engine";
	applicationInfo.engineVersion = VK_API_VERSION_1_0;
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &applicationInfo;
	createInfo.enabledExtensionCount = static_cast<std::uint32_t>(enabledExtensions.size());
	createInfo.ppEnabledExtensionNames = enabledExtensions.data();

#if !defined(NDEBUG)
	constexpr bool enableValidationLayers = true;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	if (!CheckValidationLayerSupport(validationLayers))
		throw std::runtime_error("Failed to enable validation layer support");

	createInfo.enabledLayerCount = static_cast<std::uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
#endif

	ThrowIfFailed(vkCreateInstance(&createInfo, nullptr, &m_instance));
}
VulkanInstance::~VulkanInstance()
{
	vkDestroyInstance(m_instance, nullptr);
}

VulkanInstance::operator VkInstance() const
{
	return m_instance;
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
