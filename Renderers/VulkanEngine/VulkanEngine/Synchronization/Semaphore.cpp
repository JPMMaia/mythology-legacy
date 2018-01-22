#include "pch.h"
#include "Semaphore.h"

using namespace VulkanEngine;

Semaphore::Semaphore(const vk::Device& device) :
	m_semaphore(CreateSemaphore(device))
{
}

Semaphore::operator const vk::Semaphore&() const
{
	return m_semaphore.get();
}

vk::UniqueSemaphore Semaphore::CreateSemaphore(const vk::Device& device)
{
	vk::SemaphoreCreateInfo semaphoreInfo;
	return device.createSemaphoreUnique(semaphoreInfo);
}
