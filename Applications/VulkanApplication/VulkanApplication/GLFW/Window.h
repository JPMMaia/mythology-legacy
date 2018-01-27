#pragma once

#include "Common/Events/Event.h"

namespace VulkanApplication
{
	class Window
	{
	private:
		template <class T>
		using UniquePointer = std::unique_ptr<T, std::function<void(T*)>>;

	public:
		Common::Event<Window, std::string, int, int> OnResize;

	public:
		Window(const std::string& name, int width, int height);
		Window(const Window&) = delete;
		Window(Window&&) = delete;

	public:
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

	public:
		GLFWwindow* get();
		const GLFWwindow* get() const;

	private:
		static UniquePointer<GLFWwindow> CreateWindow(Window& wrapper, const std::string& name, int width, int height);
		static void OnResizeCallback(GLFWwindow* window, int width, int height);

	private:
		UniquePointer<GLFWwindow> m_window;
	};
}
