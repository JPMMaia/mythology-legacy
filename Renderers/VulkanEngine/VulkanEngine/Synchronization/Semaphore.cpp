#include "pch.h"
#include "Semaphore.h"

#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanEngine;

Semaphore::Semaphore(VkDevice device) :
	m_device(device),
	m_semaphore(CreateSemaphore(device))
{
}
Semaphore::~Semaphore()
{
	if (m_semaphore)
		vkDestroySemaphore(m_device, m_semaphore, nullptr);
}

Semaphore::operator VkSemaphore() const
{
	return m_semaphore;
}

VkSemaphore Semaphore::CreateSemaphore(VkDevice device)
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkSemaphore semaphore;
	ThrowIfFailed(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore));
	return semaphore;
}
