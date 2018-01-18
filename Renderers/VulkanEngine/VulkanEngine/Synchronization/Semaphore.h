#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class Semaphore
	{
	public:
		explicit Semaphore(VkDevice device);
		~Semaphore();

	public:
		operator VkSemaphore() const;

	private:
		static VkSemaphore CreateSemaphore(VkDevice device);

	private:
		VkDevice m_device;
		VkSemaphore m_semaphore;
	};
}