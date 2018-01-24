#include "pch.h"
#include "DebugMessageHandler.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

#if !defined(NDEBUG)
#include <stdexcept>
#endif

using namespace VulkanEngine;

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageHandler::Callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, std::uint64_t obj, std::size_t location, std::int32_t code, const char* layerPrefix, const char* msg, void* userData)
{
	std::cerr << "Validation layer: " << msg << std::endl;

#if !defined(NDEBUG)
	if (flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_DEBUG_BIT_EXT
		|| flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_ERROR_BIT_EXT
		|| flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_WARNING_BIT_EXT
		|| flags & VkDebugReportFlagBitsEXT::VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
		throw std::runtime_error(msg);
#endif

	return VK_FALSE;
}

DebugMessageHandler::DebugMessageHandler(const vk::Instance& instance) :
	m_instance(instance),
	m_callback(CreateDebugReportCallback(instance))
{
}
DebugMessageHandler::~DebugMessageHandler()
{
	if (m_callback)
	{
		auto destroyCallback = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT"));
		if (destroyCallback != nullptr)
			destroyCallback(m_instance, m_callback, nullptr);
	}
}

VkDebugReportCallbackEXT DebugMessageHandler::CreateDebugReportCallback(const vk::Instance& instance)
{
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
	createInfo.pfnCallback = Callback;

	auto createCallback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
	if (createCallback == nullptr)
		throw std::runtime_error("Failed to create Debug Callback!");

	VkDebugReportCallbackEXT callback;
	ThrowIfFailed(createCallback(instance, &createInfo, nullptr, &callback));
	return callback;
}
