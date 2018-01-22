#include "pch.h"
#include "GLFWSurfaceBuilder.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

#include <GLFW/glfw3.h>

using namespace VulkanApplication;
using namespace VulkanEngine;

GLFWSurfaceBuilder::GLFWSurfaceBuilder(GLFWwindow& window) :
	m_window(window)
{
}

vk::UniqueSurfaceKHR GLFWSurfaceBuilder::CreateSurface(const vk::Instance& instance) const
{
	VkSurfaceKHR surface;
	ThrowIfFailed(glfwCreateWindowSurface(instance, &m_window, nullptr, &surface));
	return vk::UniqueSurfaceKHR(surface, vk::SurfaceKHRDeleter(instance));
}

std::pair<int, int> GLFWSurfaceBuilder::GetSurfaceSize() const
{
	int width, height;
	glfwGetWindowSize(&m_window, &width, &height);
	return std::make_pair(width, height);
}
