#pragma once

#include <vulkan/vulkan.h>

#include <utility>

namespace VulkanEngine
{
	class ISurfaceBuilder
	{
	public:
		virtual VkSurfaceKHR CreateSurface(VkInstance instance) const = 0;
		virtual std::pair<int, int> GetSurfaceSize() const = 0;
	};
}
