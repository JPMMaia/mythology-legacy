#include "pch.h"
#include "DebugMessageHandler.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

#include <iostream>

#if !defined(NDEBUG)
#include <stdexcept>
#endif

using namespace VulkanEngine;

VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageHandler::Callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char * layerPrefix, const char * msg, void * userData)
{
	std::cerr << "Validation layer: " << msg << std::endl;

#if !defined(NDEBUG)
	throw std::runtime_error(msg);
#endif

	return VK_FALSE;
}

DebugMessageHandler::DebugMessageHandler(VkInstance instance) :
	m_instance(instance)
{
	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = Callback;

	auto callback = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"));
	if (callback == nullptr)
		throw std::runtime_error("Failed to create Debug Callback!");

	ThrowIfFailed(callback(instance, &createInfo, nullptr, &m_callback));
}
DebugMessageHandler::~DebugMessageHandler()
{
	auto callback = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT"));
	if (callback != nullptr)
		callback(m_instance, m_callback, nullptr);
}
