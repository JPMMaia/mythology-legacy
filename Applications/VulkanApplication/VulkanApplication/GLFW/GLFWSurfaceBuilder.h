#pragma once

#include "VulkanEngine/Surfaces/ISurfaceBuilder.h"

#include <GLFW/glfw3.h>

namespace VulkanApplication
{
	class GLFWSurfaceBuilder : public VulkanEngine::ISurfaceBuilder
	{
	public:
		explicit GLFWSurfaceBuilder(GLFWwindow& window);

	public:
		vk::UniqueSurfaceKHR CreateSurface(const vk::Instance& instance) const override;
		std::pair<int, int> GetSurfaceSize() const override;

	private:
		GLFWwindow& m_window;
	};
}
