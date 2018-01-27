#include "pch.h"
#include "Application.h"
#include "Common/Timers/Timer.h"

using namespace VulkanApplication;
using namespace VulkanEngine;

Application::Application() :
	m_glfw(),
	m_window("Vulkan Application", 800, 600),
	m_renderer(CreateRenderer(m_glfw, *m_window.get()))
{
	m_window.OnResize += { "Application", this, &Application::OnWindowResizeCallback };
}

void Application::Run()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported" << std::endl;

	Common::Timer timer;
	timer.Reset();

	while (!glfwWindowShouldClose(m_window.get()))
	{
		glfwPollEvents();

		if (m_renderer->IsNextFrameAvailable())
		{
			m_renderer->FrameUpdate(timer);
			m_renderer->Render(timer);
		}
	}
}

std::unique_ptr<VulkanEngine::Renderer> Application::CreateRenderer(const GLFWManager& glfw, GLFWwindow& window)
{
	return std::make_unique<Renderer>(
		glfw.GetExtensions(),
		glfw.CreateSurfaceBuilder(window)
		);
}

void Application::OnWindowResizeCallback(int width, int height)
{
	m_renderer->CreateWindowSizeDependentResources();
}
