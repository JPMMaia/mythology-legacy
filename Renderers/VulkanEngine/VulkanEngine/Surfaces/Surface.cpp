#include "pch.h"
#include "Surface.h"

using namespace VulkanEngine;

Surface::Surface(const ISurfaceBuilder& surfaceBuilder, VkInstance instance) :
	m_instance(instance),
	m_surface(surfaceBuilder.CreateSurface(instance)),
	m_surfaceSize(surfaceBuilder.GetSurfaceSize())
{
}
Surface::~Surface()
{
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
}

int Surface::GetWidth() const
{
	return m_surfaceSize.first;
}
int Surface::GetHeight() const
{
	return m_surfaceSize.second;
}

Surface::operator VkSurfaceKHR() const
{
	return m_surface;
}
