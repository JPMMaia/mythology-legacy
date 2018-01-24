#pragma once

#include "VulkanEngine/Surfaces/ISurfaceBuilder.h"

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