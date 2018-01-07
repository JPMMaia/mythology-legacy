#pragma once

#include "VulkanEngine/Surfaces/ISurfaceBuilder.h"

namespace VulkanEngine
{
	class Surface
	{
	public:
		Surface(const ISurfaceBuilder& surfaceBuilder, VkInstance instance);

	private:
		VkSurfaceKHR m_surface;
	};
}
