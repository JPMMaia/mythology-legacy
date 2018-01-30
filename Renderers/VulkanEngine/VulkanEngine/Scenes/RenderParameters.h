#pragma once

#include "RenderLayer.h"

namespace RenderEngine
{
	struct RenderParameters
	{
		vk::CommandBuffer CommandBuffer;
		VulkanEngine::RenderLayer RenderLayer;
	};
}