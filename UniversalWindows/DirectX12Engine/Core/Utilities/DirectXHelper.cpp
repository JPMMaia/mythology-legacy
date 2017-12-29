#include "pch.h"
#include "DirectXHelper.h"
#include "Common/EngineException.h"

bool DX::Failed(HRESULT hr)
{
	return hr < 0;
}

void DX::ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		std::array<char, 4096> buffer;
		::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
			buffer.data(),
			static_cast<DWORD>(buffer.size()),
			nullptr);

		Common::EngineException exception(buffer.data());

		// Set a breakpoint on this line to catch Win32 API errors.
		throw exception;
	}
}
