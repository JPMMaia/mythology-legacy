#pragma once

#include "VulkanEngine/Surfaces/ISurfaceBuilder.h"

namespace VulkanEngine
{
	class Surface
	{
	public:
		Surface(const ISurfaceBuilder& surfaceBuilder, const vk::Instance& instance);

	public:
		int GetWidth() const;
		int GetHeight() const;

	public:
		operator const vk::SurfaceKHR&() const;

	private:
		vk::UniqueSurfaceKHR m_surface;
		std::pair<int, int> m_surfaceSize;
	};
}
