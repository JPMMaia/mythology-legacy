#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class DebugMessageHandler
	{
	public:
		static VKAPI_ATTR VkBool32 VKAPI_CALL Callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData);

	public:
		DebugMessageHandler(VkInstance instance);
		~DebugMessageHandler();

	private:
		VkInstance m_instance;
		VkDebugReportCallbackEXT m_callback;
	};
}
