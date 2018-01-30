#pragma once

#include <stdexcept>
#include <functional>
#include <memory>

#include <vulkan/vulkan.hpp>

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

	template <class IndexType>
	inline vk::IndexType ToVulkanIndexType()
	{
		if constexpr(std::is_same_v<IndexType, std::uint16_t>)
			return vk::IndexType::eUint16;
		return vk::IndexType::eUint32;
	}
}