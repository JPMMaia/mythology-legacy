#include "pch.h"
#include "VulkanRenderer.h"

#include <iostream>
#include <vector>

using namespace VulkanEngine;

Renderer::Renderer(const std::vector<const char*>& enabledExtensions, const ISurfaceBuilder& surfaceBuilder) :
	m_instance(enabledExtensions),
#if !defined(NDEBUG)
	m_debugMessageHandler(m_instance),
#endif
	m_surface(surfaceBuilder, m_instance),
	m_deviceManager(m_instance, m_surface),
	m_pipelineStateManager(m_deviceManager.GetDevice(), m_deviceManager.GetSwapSurfaceFormat().format, static_cast<float>(m_surface.GetWidth()), static_cast<float>(m_surface.GetHeight()), m_deviceManager.GetSwapExtent()),
	m_swapChain(m_deviceManager.GetDevice(), m_surface, m_deviceManager, m_pipelineStateManager.GetRenderPass())
{
}

void Renderer::CreateDeviceDependentResources()
{
}
void Renderer::CreateWindowSizeDependentResources()
{
}

bool Renderer::FrameUpdate(const Common::Timer& timer)
{
	return false;
}
bool Renderer::Render(const Common::Timer& timer)
{
	return false;
}

bool Renderer::IsNextFrameAvailable()
{
	return true;
}

void Renderer::EnumerateAvailableExtensions()
{
	std::uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "available extensions:" << std::endl;

	for (const auto& extension : extensions)
		std::cout << "\t" << extension.extensionName << std::endl;
}
