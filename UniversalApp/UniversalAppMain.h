#pragma once

#include "Common\StepTimer.h"
#include "GraphicsEngine\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"

// Renders Direct3D content on the screen.
namespace UniversalApp
{
	class UniversalAppMain
	{
	public:
		UniversalAppMain();
		void CreateRenderers(const std::shared_ptr<GraphicsEngine::DeviceResources>& deviceResources);
		void Update();
		bool Render();

		void OnWindowSizeChanged();
		void OnSuspending();
		void OnResuming();
		void OnDeviceRemoved();

	private:
		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;

		// Rendering loop timer.
		DX::StepTimer m_timer;
	};
}