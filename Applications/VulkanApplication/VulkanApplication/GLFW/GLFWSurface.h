#pragma once

#include "VulkanEngine/Surfaces/ISurfaceBuilder.h"

namespace VulkanApplication
{
	class GLFWSurface : public VulkanEngine::ISurface
	{
	public:
		explicit GLFWSurface(GLFWwindow& window);

	public:
		vk::UniqueSurfaceKHR CreateSurface(const vk::Instance& instance) const override;
		vk::Extent2D GetSurfaceExtent() const override;

	private:
		GLFWwindow& m_window;
	};
}
