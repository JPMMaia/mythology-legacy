﻿#include "pch.h"
#include "DirectX12XamlAppMain.h"

using namespace Common;
using namespace DirectX12XamlApp;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
DirectX12XamlAppMain::DirectX12XamlAppMain() :
	m_pointerLocationX(0.0f),
	m_timer(12.0f)
{
	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

// Creates and initializes the renderers.
void DirectX12XamlAppMain::CreateRenderers(const std::shared_ptr<GraphicsEngine::DeviceResources>& deviceResources)
{
	// TODO: Replace this with your app's content initialization.
	m_sceneRenderer = std::unique_ptr<GraphicsEngine::Sample3DSceneRenderer>(new GraphicsEngine::Sample3DSceneRenderer(deviceResources));

	OnWindowSizeChanged();
}

void DirectX12XamlAppMain::StartRenderLoop(const std::shared_ptr<GraphicsEngine::DeviceResources>& deviceResources)
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	m_timer.Reset();

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this, deviceResources](IAsyncAction ^ action)
	{
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			if (deviceResources->IsDeviceRemoved())
			{
				// Exit the render thread. A new thread will be created when
				// a new device is available.
				break;
			}

			auto update = [this, &deviceResources](const Common::Timer& timer)
			{
				auto commandQueue = deviceResources->GetCommandQueue();

				PIXBeginEvent(commandQueue, 0, L"Update");
				{
					Update(m_timer);
				}
				PIXEndEvent(commandQueue);
			};

			auto render = [this, &deviceResources](const Timer& timer)
			{
				auto commandQueue = deviceResources->GetCommandQueue();

				PIXBeginEvent(commandQueue, 0, L"Render");
				{
					if (Render(timer))
					{
						deviceResources->Present();
					}
				}
				PIXEndEvent(commandQueue);
			};

			auto processInput = [this]()
			{
				ProcessInput();

				return true;
			};

			auto processFrameStatistics = [this](const Timer& timer)
			{
			};

			m_timer.UpdateAndRender(update, render, processInput, processFrameStatistics);
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void DirectX12XamlAppMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

// Updates the application state once per frame.
void DirectX12XamlAppMain::Update(const Common::Timer& timer)
{
	// Update scene objects.
	m_sceneRenderer->Update(m_timer);
}

// Process all input from the user before updating game state
void DirectX12XamlAppMain::ProcessInput()
{
	// TODO: Add per frame input handling here.
	m_sceneRenderer->TrackingUpdate(m_pointerLocationX);
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool DirectX12XamlAppMain::Render(const Common::Timer& timer)
{
	// Render the scene objects.
	return m_sceneRenderer->Render();
}

// Updates application state when the window size changes (e.g. device orientation change)
void DirectX12XamlAppMain::OnWindowSizeChanged()
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// Notifies the app that it is being suspended.
void DirectX12XamlAppMain::OnSuspending()
{
	// TODO: Replace this with your app's suspending logic.

	// Process lifetime management may terminate suspended apps at any time, so it is
	// good practice to save any state that will allow the app to restart where it left off.

	m_sceneRenderer->SaveState();

	// If your application uses video memory allocations that are easy to re-create,
	// consider releasing that memory to make it available to other applications.
}

// Notifes the app that it is no longer suspended.
void DirectX12XamlAppMain::OnResuming()
{
	// TODO: Replace this with your app's resuming logic.
}

// Notifies renderers that device resources are about to be released.
void DirectX12XamlAppMain::OnDeviceRemoved()
{
	m_sceneRenderer->SaveState();
	m_sceneRenderer = nullptr;
}
