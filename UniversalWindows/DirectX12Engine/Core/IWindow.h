#pragma once

#include <DirectXMath.h>

struct IUnknown;

namespace DirectX12Engine
{
	enum class DisplayOrientations;

	class IWindow
	{
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
