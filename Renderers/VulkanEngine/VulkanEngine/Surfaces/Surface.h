#pragma once

#include "VulkanEngine/Surfaces/ISurfaceBuilder.h"

namespace VulkanEngine
{
	class Surface
	{
	public:
		Surface(std::unique_ptr<ISurface> surfaceInterface, const vk::Instance& instance);

	public:
		vk::Extent2D GetExtent() const;

	public:
		operator const vk::SurfaceKHR&() const;

	private:
		std::unique_ptr<ISurface> m_surfaceInterface;
		vk::UniqueSurfaceKHR m_surface;
	};
}
