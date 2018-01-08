#pragma once

namespace VulkanEngine
{
	struct QueueFamilyIndices
	{
		int GraphicsFamily = -1;
		int PresentFamily = -1;

		bool IsComplete() const
		{
			return GraphicsFamily >= 0 && PresentFamily >= 0;
		}
	};
}