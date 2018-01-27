#include "pch.h"
#include "Window.h"

using namespace VulkanApplication;

Window::Window(const std::string& name, int width, int height) :
	m_window(CreateWindow(*this, name, width, height))
{
}

GLFWwindow* VulkanApplication::Window::get()
{
	return m_window.get();
}
const GLFWwindow* VulkanApplication::Window::get() const
{
	return m_window.get();
}


Window::UniquePointer<GLFWwindow> Window::CreateWindow(Window& wrapper, const std::string& name, int width, int height)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	auto window = UniquePointer<GLFWwindow>(
		glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr),
		[](GLFWwindow* window) { glfwDestroyWindow(window); }
	);

	glfwSetWindowUserPointer(window.get(), &wrapper);
	glfwSetWindowSizeCallback(window.get(), Window::OnResizeCallback);

	return window;
}

void Window::OnResizeCallback(GLFWwindow* window, int width, int height)
{
	auto* wrapper = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	wrapper->OnResize(width, height);
}
