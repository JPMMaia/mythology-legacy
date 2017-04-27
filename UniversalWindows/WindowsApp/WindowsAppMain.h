#pragma once

#include "Common/Timer.h"
#include "../DirectX12Engine/Core/DeviceResources.h"

// Renders Direct3D content on the screen.
namespace WindowsApp
{
	class WindowsAppMain
	{
	public:
		WindowsAppMain();
		void CreateRenderers(const std::shared_ptr<DirectX12Engine::DeviceResources>& deviceResources);
		void Update();
		bool Render();

		void OnWindowSizeChanged();
		void OnSuspending();
		void OnResuming();
		void OnDeviceRemoved();

	private:
		// Rendering loop timer.
		Common::Timer m_timer;
	};
}