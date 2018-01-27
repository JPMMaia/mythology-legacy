#pragma once

#include "GLFW/GLFWManager.h"
#include "GLFW/Window.h"
#include "VulkanEngine/VulkanRenderer.h"

namespace VulkanApplication
{
	class Application
	{
	public:
		Application();

	public:
		void Run();

	private:
		static std::unique_ptr<VulkanEngine::Renderer> CreateRenderer(const GLFWManager& glfw, GLFWwindow& window);

	private:
		void OnWindowResizeCallback(int width, int height);

	private:
		GLFWManager m_glfw;
		Window m_window;
		std::unique_ptr<VulkanEngine::Renderer> m_renderer;

	private:
		static constexpr int c_width = 800;
		static constexpr int c_height = 600;
	};
}
