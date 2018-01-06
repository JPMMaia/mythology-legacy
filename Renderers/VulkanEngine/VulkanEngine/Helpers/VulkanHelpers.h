#pragma once

#include <stdexcept>
#include <functional>
#include <memory>

#include <vulkan/vulkan.h>

namespace VulkanEngine
{	
	template<class T>
	using VulkanUniquePointer = std::unique_ptr<T, std::function<void(T*)>>;

	template<class T>
	using VulkanSharedPointer = std::shared_ptr<T>;

	inline void ThrowIfFailed(VkResult result)
	{
		if (result != VK_SUCCESS)
			throw std::runtime_error("An error related with Vulkan occurred!");
	}
}