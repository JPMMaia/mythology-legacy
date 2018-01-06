#pragma once

#include <vector>

namespace VulkanApplication
{
	class GLFWManager
	{
	public:
		GLFWManager();
		~GLFWManager();

	public:
		std::vector<const char*> GetExtensions() const;
	};
}