#pragma once

#include <vector>

#include <Eigen/Eigen>
#include <vulkan/vulkan.hpp>

namespace VulkanEngine
{
	struct Vertex
	{
		Eigen::Vector2f Position;
		Eigen::Vector3f Color;

		static vk::VertexInputBindingDescription GetBindingDescription() 
		{
			return vk::VertexInputBindingDescription(
				0,
				sizeof(Vertex),
				vk::VertexInputRate::eVertex
			);
		}

		static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions()
		{
			return 
			{
				vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, Position)),
				vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, Color)),
			};
		}
	};
}
