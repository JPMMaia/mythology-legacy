#include "pch.h"
#include "Surface.h"

using namespace VulkanEngine;

Surface::Surface(const ISurfaceBuilder& surfaceBuilder, const vk::Instance& instance) :
	m_surface(surfaceBuilder.CreateSurface(instance)),
	m_surfaceSize(surfaceBuilder.GetSurfaceSize())
{
}

int Surface::GetWidth() const
{
	return m_surfaceSize.first;
}
int Surface::GetHeight() const
{
	return m_surfaceSize.second;
}

Surface::operator const vk::SurfaceKHR&() const
{
	return m_surface.get();
}
