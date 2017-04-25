#pragma once

#include "GraphicsEngineInterfaces/DisplayOrientations.h"
#include "GraphicsEngineInterfaces/IWindow.h"

namespace WindowsApp
{
	namespace Converters
	{
		inline GraphicsEngine::DisplayOrientations UWPToNative(Windows::Graphics::Display::DisplayOrientations orientation)
		{
			using namespace Windows::Graphics::Display;
			switch (orientation)
			{
			case DisplayOrientations::None: return GraphicsEngine::DisplayOrientations::None;
			case DisplayOrientations::Landscape: return GraphicsEngine::DisplayOrientations::Landscape;
			case DisplayOrientations::Portrait: return GraphicsEngine::DisplayOrientations::Portrait;
			case DisplayOrientations::LandscapeFlipped: return GraphicsEngine::DisplayOrientations::LandscapeFlipped;
			case DisplayOrientations::PortraitFlipped: return GraphicsEngine::DisplayOrientations::PortraitFlipped;
			default: return GraphicsEngine::DisplayOrientations::None;
			}
		}

		class NativeWindow : public GraphicsEngine::IWindow
		{
		public:
			explicit NativeWindow(Windows::UI::Core::CoreWindow^ window) :
				m_window(window)
			{
			}

			IUnknown* Get() override
			{
				return reinterpret_cast<IUnknown*>(m_window.Get());
			}
			DirectX::XMFLOAT2 Size() override
			{
				return DirectX::XMFLOAT2(m_window->Bounds.Width, m_window->Bounds.Height);
			}
			GraphicsEngine::DisplayOrientations NativeOrientation() override
			{
				auto currentDisplayInformation = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
				return UWPToNative(currentDisplayInformation->NativeOrientation);
			}
			GraphicsEngine::DisplayOrientations CurrentOrientation() override
			{
				auto currentDisplayInformation = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
				return UWPToNative(currentDisplayInformation->CurrentOrientation);
			}
			float LogicalDpi() override
			{
				auto currentDisplayInformation = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
				return currentDisplayInformation->LogicalDpi;
			}

		private:
			Platform::Agile<Windows::UI::Core::CoreWindow> m_window;
		};
	}
}