#include "pch.h"
#include "WindowsAppMain.h"
#include "Interfaces/IFileSystem.h"

using namespace Common;
using namespace WindowsApp;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

namespace WindowsApp
{
	class FileSystem : public GameEngine::IFileSystem
	{
	public:
		std::wstring GetWorkingDirectory() const override
		{
			Platform::String^ workingDirectory = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
			return std::wstring(workingDirectory->Begin()) + L"\\";
		}
	};
}
	

WindowsAppMain::WindowsAppMain() :
	m_timer(std::chrono::milliseconds(12)),
	m_game(std::make_shared<Mythology::MythologyGame>(std::make_shared<FileSystem>()))
{
}

// Creates and initializes the renderers.
void WindowsAppMain::CreateRenderers(const std::shared_ptr<DirectX12Engine::DeviceResources>& deviceResources)
{
	m_deviceResources = deviceResources;
	m_renderer = std::make_unique<DirectX12Engine::Renderer>(deviceResources);
	m_scene = std::make_shared<DirectX12Engine::StandardScene>(deviceResources, m_renderer->GetCommandListManager(), m_game);
	m_scene->CreateDeviceDependentResources();
	m_renderer->SetScene(m_scene);

	OnWindowSizeChanged();

	m_timer.Reset();
}

bool WindowsAppMain::UpdateAndRender()
{
	using namespace std::placeholders;

	return m_timer.UpdateAndRender(
		std::bind(&WindowsAppMain::FixedUpdate, this, _1), 
		std::bind(&WindowsAppMain::FrameUpdate, this, _1), 
		std::bind(&WindowsAppMain::Render, this, _1), 
		std::bind(&WindowsAppMain::ProcessInput, this), 
		std::bind(&WindowsAppMain::ProcessFrameStatistics, this, _1)
	);
}

// Updates application state when the window's size changes (e.g. device orientation change)
void WindowsAppMain::OnWindowSizeChanged()
{
	m_renderer->CreateWindowSizeDependentResources();
	m_scene->CreateWindowSizeDependentResources();
}

// Notifies the app that it is being suspended.
void WindowsAppMain::OnSuspending()
{
	// TODO: Replace this with your app's suspending logic.

	// Process lifetime management may terminate suspended apps at any time, so it is
	// good practice to save any state that will allow the app to restart where it left off.

	//m_sceneRenderer->SaveState();

	// If your application uses video memory allocations that are easy to re-create,
	// consider releasing that memory to make it available to other applications.
}

// Notifes the app that it is no longer suspended.
void WindowsAppMain::OnResuming()
{
	// TODO: Replace this with your app's resuming logic.
}

// Notifies renderers that device resources need to be released.
void WindowsAppMain::OnDeviceRemoved()
{
	// TODO: Save any necessary application or renderer state and release the renderer
	// and its resources which are no longer valid.
	//m_sceneRenderer->SaveState();
	//m_sceneRenderer = nullptr;
}

bool WindowsAppMain::ProcessInput()
{
	m_game->ProcessInput();

	m_renderer->ProcessInput();

	return true;
}
void WindowsAppMain::FixedUpdate(const Common::Timer& timer)
{
	m_game->FixedUpdate(timer);
}
void WindowsAppMain::FrameUpdate(const Common::Timer& timer)
{	
	m_renderer->FrameUpdate(timer);
}
void WindowsAppMain::Render(const Common::Timer& timer)
{	
	m_renderer->Render(timer);
}
void WindowsAppMain::ProcessFrameStatistics(const Common::Timer& timer)
{	
}