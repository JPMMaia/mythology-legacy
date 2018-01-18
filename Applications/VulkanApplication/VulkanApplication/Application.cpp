#include "pch.h"
#include "Application.h"
#include "Common/Timers/Timer.h"

#include <iostream>

using namespace VulkanApplication;
using namespace VulkanEngine;

Application::Application()
{
	InitializeWindow();
	InitializeRenderer();
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

void Application::InitializeWindow()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = VulkanUniquePointer<GLFWwindow>(
		glfwCreateWindow(c_width, c_height, "Vulkan window", nullptr, nullptr),
		[](GLFWwindow* window) { glfwDestroyWindow(window); 	}
	);
}
void Application::InitializeRenderer()
{
	m_renderer = std::make_unique<Renderer>(m_glfw.GetExtensions(), *m_glfw.CreateSurfaceBuilder(*m_window));
	m_renderer->CreateDeviceDependentResources();
	m_renderer->CreateWindowSizeDependentResources();
}
