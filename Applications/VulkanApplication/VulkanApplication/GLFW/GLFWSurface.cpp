#include "pch.h"
#include "GLFWSurface.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanApplication;
using namespace VulkanEngine;

GLFWSurface::GLFWSurface(GLFWwindow& window) :
	m_window(window)
{
}

vk::UniqueSurfaceKHR GLFWSurface::CreateSurface(const vk::Instance& instance) const
{
	VkSurfaceKHR surface;
	ThrowIfFailed(glfwCreateWindowSurface(instance, &m_window, nullptr, &surface));
	return vk::UniqueSurfaceKHR(surface, vk::SurfaceKHRDeleter(instance));
}

vk::Extent2D GLFWSurface::GetSurfaceExtent() const
{
	int width, height;
	glfwGetWindowSize(&m_window, &width, &height);
	return { static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height) };
}
