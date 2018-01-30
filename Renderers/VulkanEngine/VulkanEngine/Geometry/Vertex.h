#pragma once

#include <vector>

#include <Eigen/Eigen>
#include <vulkan/vulkan.hpp>

#include "RenderEngine/Geometry/Vertex.h"

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

		static std::vector<Vertex> CreateFromVertexData(const std::vector<RenderEngine::Vertex>& vertexData)
		{
			std::vector<Vertex> vertices;
			vertices.reserve(vertexData.size());

			for (const auto& vertex : vertexData)
				vertices.push_back({ { vertex.Position.x(), vertex.Position.y() }, vertex.Normal });

			return vertices;
		}
	};
}
