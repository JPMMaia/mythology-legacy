#include "pch.h"
#include "Surface.h"

using namespace VulkanEngine;

Surface::Surface(std::unique_ptr<ISurface> surfaceInterface, const vk::Instance& instance) :
	m_surfaceInterface(std::move(surfaceInterface)),
	m_surface(m_surfaceInterface->CreateSurface(instance))
{
}

vk::Extent2D Surface::GetExtent() const
{
	return m_surfaceInterface->GetSurfaceExtent();
}

Surface::operator const vk::SurfaceKHR&() const
{
	return m_surface.get();
}
