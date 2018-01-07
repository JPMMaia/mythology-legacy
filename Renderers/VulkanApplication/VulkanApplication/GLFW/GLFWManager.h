#pragma once

#include "VulkanEngine/Surfaces/ISurfaceBuilder.h"

#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

namespace VulkanApplication
{
	class GLFWManager
	{
	public:
		GLFWManager();
		~GLFWManager();

	public:
		std::vector<const char*> GetExtensions() const;
		std::unique_ptr<VulkanEngine::ISurfaceBuilder> CreateSurfaceBuilder(GLFWwindow& window) const;
	};
}