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

VkSurfaceKHR GLFWSurfaceBuilder::CreateSurface(VkInstance instance) const
{
	VkSurfaceKHR surface;
	ThrowIfFailed(glfwCreateWindowSurface(instance, &m_window, nullptr, &surface));
	return surface;
}

std::pair<int, int> GLFWSurfaceBuilder::GetSurfaceSize() const
{
	int width, height;
	glfwGetWindowSize(&m_window, &width, &height);
	return std::make_pair(width, height);
}
