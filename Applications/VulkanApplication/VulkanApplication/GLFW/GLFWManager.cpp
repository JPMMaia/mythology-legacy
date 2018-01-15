#include "pch.h"
#include "GLFWManager.h"
#include "GLFWSurfaceBuilder.h"

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

	std::vector<const char*> output;
	output.reserve(count + 1);

	for (std::uint32_t i = 0; i < count; ++i)
		output.emplace_back(extensions[i]);

#if !defined(NDEBUG)
	output.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif

	return output;
}

std::unique_ptr<VulkanEngine::ISurfaceBuilder> GLFWManager::CreateSurfaceBuilder(GLFWwindow& window) const
{
	return std::make_unique<GLFWSurfaceBuilder>(window);
}
