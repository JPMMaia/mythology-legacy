#pragma once

#include "GraphicsEngine/IWindow.h"

namespace UniversalApp
{
	namespace Converters
	{
		inline GraphicsEngine::IWindow::DisplayOrientations UWPToNative(Windows::Graphics::Display::DisplayOrientations orientation)
		{
			using namespace GraphicsEngine;
			using namespace Windows::Graphics::Display;
			switch (orientation)
			{
			case DisplayOrientations::None: return IWindow::DisplayOrientations::None;
			case DisplayOrientations::Landscape: return IWindow::DisplayOrientations::Landscape;
			case DisplayOrientations::Portrait: return IWindow::DisplayOrientations::Portrait;
			case DisplayOrientations::LandscapeFlipped: return IWindow::DisplayOrientations::LandscapeFlipped;
			case DisplayOrientations::PortraitFlipped: return IWindow::DisplayOrientations::PortraitFlipped;
			default: return IWindow::DisplayOrientations::None;
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
			DisplayOrientations NativeOrientation() override
			{
				auto currentDisplayInformation = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
				return UWPToNative(currentDisplayInformation->NativeOrientation);
			}
			DisplayOrientations CurrentOrientation() override
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