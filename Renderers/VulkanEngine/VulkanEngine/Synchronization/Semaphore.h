#pragma once

namespace VulkanEngine
{
	class Semaphore
	{
	public:
		explicit Semaphore(const vk::Device& device);

	public:
		operator const vk::Semaphore&() const;

	private:
		static vk::UniqueSemaphore CreateSemaphore(const vk::Device& device);

	private:
		vk::UniqueSemaphore m_semaphore;
	};
}