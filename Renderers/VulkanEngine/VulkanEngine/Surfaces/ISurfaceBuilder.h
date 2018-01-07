#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class ISurfaceBuilder
	{
	public:
		virtual VkSurfaceKHR CreateSurface(VkInstance instance) const = 0;
	};
}
