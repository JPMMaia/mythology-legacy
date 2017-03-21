#pragma once

#include "GraphicsEngine/ISwapChainPanel.h"
#include "Common/Helpers.h"

#include <agile.h>
#include <windows.ui.xaml.media.dxinterop.h>

namespace DirectX12XamlApp
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

		class NativeSwapChainPanel : public GraphicsEngine::ISwapChainPanel
		{
		public:
			explicit NativeSwapChainPanel(Windows::UI::Xaml::Controls::SwapChainPanel^ swapChainPanel) :
				m_swapChainPanel(swapChainPanel)
			{
			}

			IUnknown* Get() override
			{
				return reinterpret_cast<IUnknown*>(m_swapChainPanel.Get());
			}
			DirectX::XMFLOAT2 Size() override
			{
				return DirectX::XMFLOAT2(static_cast<float>(m_swapChainPanel->ActualWidth), static_cast<float>(m_swapChainPanel->ActualHeight));
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
			float CompositionScaleX() override
			{
				return m_swapChainPanel->CompositionScaleX;
			}
			float CompositionScaleY() override
			{
				return m_swapChainPanel->CompositionScaleY;
			}
			float LogicalDpi() override
			{
				auto currentDisplayInformation = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
				return currentDisplayInformation->LogicalDpi;
			}

			void SetSwapChainOnPanelAsync(IDXGISwapChain3* swapChain) override
			{
				using namespace Microsoft::WRL;
				using namespace Windows::Foundation;
				using namespace Windows::Graphics::Display;
				using namespace Windows::UI::Core;
				using namespace Windows::UI::Xaml::Controls;

				// UI changes will need to be dispatched back to the UI thread.
				m_swapChainPanel->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([=]()
				{
					// Get backing native interface for SwapChainPanel.
					ComPtr<ISwapChainPanelNative> panelNative;
					Common::Helpers::ThrowIfFailed(
						reinterpret_cast<IUnknown*>(m_swapChainPanel.Get())->QueryInterface(IID_PPV_ARGS(&panelNative))
					);

					Common::Helpers::ThrowIfFailed(panelNative->SetSwapChain(swapChain));
				}, Platform::CallbackContext::Any));
			}

		private:
			Platform::Agile<Windows::UI::Xaml::Controls::SwapChainPanel> m_swapChainPanel;
		};
	}
}