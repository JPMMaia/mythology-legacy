#pragma once

#include <DirectXMath.h>

namespace GraphicsEngine
{
	class IWindow
	{
	public:
		enum class DisplayOrientations
		{

			None = 0,
			Landscape = 1,
			Portrait = 2,
			LandscapeFlipped = 4,
			PortraitFlipped = 8
		};

	public:
		IWindow() = default;
		virtual ~IWindow() = default;

	public:
		virtual IUnknown* Get() = 0;
		virtual DirectX::XMFLOAT2 Size() = 0;
		virtual DisplayOrientations NativeOrientation() = 0;
		virtual DisplayOrientations CurrentOrientation() = 0;
		virtual float LogicalDpi() = 0;
	};
}
