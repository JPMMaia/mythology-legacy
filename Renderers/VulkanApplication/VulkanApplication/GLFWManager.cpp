#include "pch.h"
#include "GLFWManager.h"

#include <GLFW/glfw3.h>

using namespace VulkanApplication;

GLFWManager::GLFWManager()
{
	glfwInit();
}
GLFWManager::~GLFWManager()
{
	glfwTerminate();
}

std::vector<const char*> GLFWManager::GetExtensions() const
{
	std::uint32_t count;
	auto extensions = glfwGetRequiredInstanceExtensions(&count);

	std::vector<const char*> output(count);
	for (std::uint32_t i = 0; i < count; ++i)
		output[i] = extensions[i];

	return output;
}
