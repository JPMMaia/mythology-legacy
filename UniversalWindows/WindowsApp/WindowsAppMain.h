﻿#pragma once

#include "Common/Timers/Timer.h"
#include "Core/DeviceResources.h"
#include "Core/Renderer.h"
#include "Core/Scene/StandardScene.h"
#include "Mythology/MythologyGame.h"

#include <shared_mutex>
#include <thread>

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

		void RunUpdate();
		void StopUpdate();

		const std::shared_ptr<Mythology::MythologyGame>& GetGame() const { return m_game; }

	private:
		bool ProcessInput();
		void FixedUpdate(const Common::Timer& timer);
		bool FrameUpdate(const Common::Timer& timer);
		void Render(const Common::Timer& timer);
		void ProcessFrameStatistics(const Common::Timer& timer);

	private:
		std::shared_ptr<DirectX12Engine::DeviceResources> m_deviceResources;
		Common::Timer m_timer;
		std::shared_ptr<Mythology::MythologyGame> m_game;
		std::unique_ptr<DirectX12Engine::Renderer> m_renderer;
		std::shared_ptr<DirectX12Engine::StandardScene> m_scene;

		std::thread m_updateThread;
		std::shared_mutex m_updateMutex;
		bool m_update;
	};
}