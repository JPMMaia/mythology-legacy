#include "pch.h"
#include "Surface.h"

using namespace VulkanEngine;

Surface::Surface(const ISurfaceBuilder& surfaceBuilder, VkInstance instance) :
	m_surface(surfaceBuilder.CreateSurface(instance))
{
}
