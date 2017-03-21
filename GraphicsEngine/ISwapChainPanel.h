#pragma once

#include "DisplayOrientations.h"

#include <DirectXMath.h>
#include <unknwn.h>
#include <dxgi1_5.h>

namespace GraphicsEngine
{
	class ISwapChainPanel
	{
	public:
		ISwapChainPanel() = default;
		virtual ~ISwapChainPanel() = default;

	public:
		virtual IUnknown* Get() = 0;
		virtual DirectX::XMFLOAT2 Size() = 0;
		virtual DisplayOrientations NativeOrientation() = 0;
		virtual DisplayOrientations CurrentOrientation() = 0;
		virtual float CompositionScaleX() = 0;
		virtual float CompositionScaleY() = 0;
		virtual float LogicalDpi() = 0;
		virtual void SetSwapChainOnPanelAsync(IDXGISwapChain3* swapChain) = 0;
	};
}
