#pragma once

#include "Common\StepTimer.h"
#include "GraphicsEngine\DeviceResources.h"
#include "GraphicsEngine\Sample3DSceneRenderer.h"
#include "Common\Timer.h"

// Renders Direct3D content on the screen.
namespace UniversalApp
{
	class UniversalAppMain
	{
	public:
		UniversalAppMain();
		void CreateRenderers(const std::shared_ptr<GraphicsEngine::DeviceResources>& deviceResources);
		void Update(const Common::Timer& timer);
		bool Render(const Common::Timer& timer);

		void OnWindowSizeChanged();
		void OnSuspending();
		void OnResuming();
		void OnDeviceRemoved();

	private:
		// TODO: Replace with your own content renderers.
		std::unique_ptr<GraphicsEngine::Sample3DSceneRenderer> m_sceneRenderer;
	};
}