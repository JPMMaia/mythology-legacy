#pragma once

#include "Core/DisplayOrientations.h"
#include "Core/IWindow.h"

namespace WindowsApp
{
	namespace Converters
	{
		inline DirectX12Engine::DisplayOrientations UWPToNative(Windows::Graphics::Display::DisplayOrientations orientation)
		{
			using namespace Windows::Graphics::Display;
			switch (orientation)
			{
			case DisplayOrientations::None: return DirectX12Engine::DisplayOrientations::None;
			case DisplayOrientations::Landscape: return DirectX12Engine::DisplayOrientations::Landscape;
			case DisplayOrientations::Portrait: return DirectX12Engine::DisplayOrientations::Portrait;
			case DisplayOrientations::LandscapeFlipped: return DirectX12Engine::DisplayOrientations::LandscapeFlipped;
			case DisplayOrientations::PortraitFlipped: return DirectX12Engine::DisplayOrientations::PortraitFlipped;
			default: return DirectX12Engine::DisplayOrientations::None;
			}
		}

		class NativeWindow : public DirectX12Engine::IWindow
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
			DirectX12Engine::DisplayOrientations NativeOrientation() override
			{
				auto currentDisplayInformation = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
				return UWPToNative(currentDisplayInformation->NativeOrientation);
			}
			DirectX12Engine::DisplayOrientations CurrentOrientation() override
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