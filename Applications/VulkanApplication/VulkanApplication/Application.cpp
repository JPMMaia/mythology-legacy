#include "pch.h"
#include "Application.h"
#include "Common/Timers/Timer.h"
#include "RenderEngine/Geometry/MeshData.h"
#include "RenderEngine/Geometry/Vertex.h"

using namespace VulkanApplication;
using namespace VulkanEngine;

Application::Application() :
	m_glfw(),
	m_window("Vulkan Application", 800, 600),
	m_renderer(CreateRenderer(m_glfw, *m_window.get())),
	m_scene(CreateScene(m_renderer))
{
	m_window.OnResize += { "Application", this, &Application::OnWindowResizeCallback };

	m_renderer->SetScene(m_scene);
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

std::shared_ptr<VulkanEngine::Renderer> Application::CreateRenderer(const GLFWManager& glfw, GLFWwindow& window)
{
	return std::make_unique<Renderer>(
		glfw.GetExtensions(),
		glfw.CreateSurfaceBuilder(window)
		);
}
std::shared_ptr<VulkanEngine::Scene> Application::CreateScene(const std::shared_ptr<VulkanEngine::Renderer>& renderer)
{
	auto scene = std::make_shared<VulkanEngine::Scene>(renderer);

	using VertexType = RenderEngine::Vertex;
	using IndexType = std::uint16_t;
	RenderEngine::MeshData<VertexType, IndexType> meshData;

	meshData.Vertices =
	{
		{ { -0.5f, -0.5f, 0.0f },{ 1.0f, 0.0f, 0.0f }, {}, {} },
		{ { -0.5f, 0.5f, 0.0f },{ 0.0f, 1.0f, 0.0f }, {}, {} },
		{ { 0.5f, -0.5f, 0.0f },{ 0.0f, 0.0f, 1.0f }, {}, {} },
		{ { 0.5f, 0.5f, 0.0f },{ 1.0f, 1.0f, 1.0f }, {}, {} }
	};
	
	meshData.Indices = 
	{
		0, 1, 2,
		2, 1, 3,
	};
	
	scene->CreateMesh("Triangle", meshData);

	return scene;
}

void Application::OnWindowResizeCallback(int width, int height)
{
	if (width == 0 || height == 0)
		return;

	m_renderer->RecreateWindowSizeDependentResources();
}
