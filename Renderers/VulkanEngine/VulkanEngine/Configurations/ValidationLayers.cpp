#include "pch.h"
#include "ValidationLayers.h"

using namespace VulkanEngine;

std::vector<const char*> ValidationLayers::Get()
{
#if !defined(NDEBUG)
	return {
		"VK_LAYER_LUNARG_standard_validation"
	};
#else
	return {};
#endif
}
