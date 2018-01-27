#pragma once

#include <vulkan/vulkan.hpp>

#include <utility>

namespace VulkanEngine
{
	class ISurface
	{
	public:
		virtual vk::UniqueSurfaceKHR CreateSurface(const vk::Instance& instance) const = 0;
		virtual vk::Extent2D GetSurfaceExtent() const = 0;
	};
}
