#pragma once

#include "GLFW/GLFWManager.h"
#include "GLFW/Window.h"
#include "VulkanEngine/Renderer.h"
#include "VulkanEngine/Scenes/Scene.h"

namespace VulkanApplication
{
	class Application
	{
	public:
		Application();

	public:
		void Run();

	private:
		static std::shared_ptr<VulkanEngine::Renderer> CreateRenderer(const GLFWManager& glfw, GLFWwindow& window);
		static std::shared_ptr<VulkanEngine::Scene> CreateScene(const std::shared_ptr<VulkanEngine::Renderer>& renderer);

	private:
		void OnWindowResizeCallback(int width, int height);

	private:
		GLFWManager m_glfw;
		Window m_window;
		std::shared_ptr<VulkanEngine::Renderer> m_renderer;
		std::shared_ptr<VulkanEngine::Scene> m_scene;

	private:
		static constexpr int c_width = 800;
		static constexpr int c_height = 600;
	};
}
