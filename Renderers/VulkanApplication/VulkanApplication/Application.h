#pragma once

#include "GLFWManager.h"
#include "VulkanEngine/VulkanRenderer.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

#include <GLFW/glfw3.h>

namespace VulkanApplication
{
	class Application
	{
	public:
		Application();

	public:
		void Run();

	private:
		void InitializeWindow();
		void InitializeRenderer();

	private:
		GLFWManager m_glfw;
		VulkanEngine::VulkanUniquePointer<GLFWwindow> m_window;
		std::unique_ptr<VulkanEngine::Renderer> m_renderer;
	};
}