#include "pch.h"
#include "DirectX12XamlAppMain.h"
#include "DirectX12\Renderer.h"

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
}

// Creates and initializes the renderers.
void DirectX12XamlAppMain::CreateRenderers(const std::shared_ptr<GraphicsEngine::DeviceResources>& deviceResources)
{
	m_renderer = std::make_unique<GraphicsEngine::Renderer>(deviceResources);

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
	Update(m_timer);

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

void DirectX12XamlAppMain::Update(const Common::Timer& timer)
{
	m_renderer->FrameUpdate(m_timer);
}
void DirectX12XamlAppMain::ProcessInput()
{
}
bool DirectX12XamlAppMain::Render(const Common::Timer& timer)
{
	return m_renderer->Render(timer);
}

void DirectX12XamlAppMain::OnWindowSizeChanged()
{
	m_renderer->CreateWindowSizeDependentResources();
}
void DirectX12XamlAppMain::OnSuspending()
{
	m_renderer->SaveState();
}
void DirectX12XamlAppMain::OnResuming()
{
	m_renderer->LoadState();
}
void DirectX12XamlAppMain::OnDeviceRemoved()
{
	m_renderer = nullptr;
}
