#pragma once

#include "Common/Timer.h"
#include "Core/DeviceResources.h"
#include "Core/Renderer.h"

// Renders Direct3D content on the screen.
namespace WindowsApp
{
	class WindowsAppMain
	{
	public:
		WindowsAppMain();
		void CreateRenderers(const std::shared_ptr<DirectX12Engine::DeviceResources>& deviceResources);
		bool UpdateAndRender();

		void OnWindowSizeChanged();
		void OnSuspending();
		void OnResuming();
		void OnDeviceRemoved();

	private:
		bool ProcessInput();
		void FixedUpdate(const Common::Timer& timer);
		void FrameUpdate(const Common::Timer& timer);
		void Render(const Common::Timer& timer);
		void ProcessFrameStatistics(const Common::Timer& timer);

	private:
		std::shared_ptr<DirectX12Engine::DeviceResources> m_deviceResources;
		Common::Timer m_timer;
		std::unique_ptr<DirectX12Engine::Renderer> m_renderer;
	};
}