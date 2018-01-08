#pragma once

#include "VulkanEngine/Surfaces/ISurfaceBuilder.h"

namespace VulkanEngine
{
	class Surface
	{
	public:
		Surface(const ISurfaceBuilder& surfaceBuilder, VkInstance instance);
		~Surface();

	public:
		int GetWidth() const;
		int GetHeight() const;

	public:
		operator VkSurfaceKHR() const;

	private:
		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		int m_width;
		int m_height;
	};
}
