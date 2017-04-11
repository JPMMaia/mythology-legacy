#pragma once

#include "GraphicsEngine/DirectX12/DeviceResources.h"
#include "GraphicsEngine/DirectX12/Scene/StandardScene.h"
#include "GraphicsEngine/Interfaces/IScene.h"
#include "GraphicsEngine/Interfaces/IRenderer.h"
#include "Common\Timer.h"

// Renders Direct3D content on the screen.
namespace DirectX12XamlApp
{
	class DirectX12XamlAppMain
	{
	public:
		DirectX12XamlAppMain();
		void CreateRenderers(const std::shared_ptr<GraphicsEngine::DeviceResources>& deviceResources);
		void StartTracking() { int i = 0; }
		void TrackingUpdate(float positionX)	{ m_pointerLocationX = positionX; }
		void StopTracking() { int i = 0; }
		bool IsTracking()						{ return false; }
		void StartRenderLoop(const std::shared_ptr<GraphicsEngine::DeviceResources>& deviceResources);
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		void OnWindowSizeChanged();
		void OnSuspending();
		void OnResuming();
		void OnDeviceRemoved();

	private:
		void ProcessInput();
		void Update(const Common::Timer& timer);
		bool Render(const Common::Timer& timer);

		std::unique_ptr<GraphicsEngine::IRenderer> m_renderer;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		Common::Timer m_timer;

		// Track current input pointer position.
		float m_pointerLocationX;
	};
}