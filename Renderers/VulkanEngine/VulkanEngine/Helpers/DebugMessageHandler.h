#pragma once

namespace VulkanEngine
{
	class DebugMessageHandler
	{
	public:
		static VKAPI_ATTR VkBool32 VKAPI_CALL Callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, std::uint64_t obj, std::size_t location, std::int32_t code, const char* layerPrefix, const char* msg, void* userData);

	public:
		explicit DebugMessageHandler(const vk::Instance& instance);
		~DebugMessageHandler();

	private:
		static VkDebugReportCallbackEXT CreateDebugReportCallback(const vk::Instance& instance);

	private:
		vk::Instance m_instance;
		VkDebugReportCallbackEXT m_callback;
	};
}
