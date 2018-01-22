#pragma once

#include <vulkan/vulkan.hpp>

#include <utility>

namespace VulkanEngine
{
	class ISurfaceBuilder
	{
	public:
		virtual vk::UniqueSurfaceKHR CreateSurface(const vk::Instance& instance) const = 0;
		virtual std::pair<int, int> GetSurfaceSize() const = 0;
	};
}
