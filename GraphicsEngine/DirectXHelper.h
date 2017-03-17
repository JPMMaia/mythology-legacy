#pragma once

#include <d3d12.h>
#include <ppltasks.h>	// For create_task

namespace DX
{
	// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
	inline float ConvertDipsToPixels(float dips, float dpi)
	{
		static const auto dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
	}

	// Assign a name to the object to aid with debugging.
#if defined(_DEBUG)
	inline void SetName(ID3D12Object* pObject, LPCWSTR name)
	{
		pObject->SetName(name);
	}
#else
	inline void SetName(ID3D12Object*, LPCWSTR)
	{
	}
#endif
}

// Naming helper for ComPtr<T>.
// Assigns the name of the variable as the name of the object.
#define NAME_D3D12_OBJECT(x) DX::SetName(x.Get(), L#x)