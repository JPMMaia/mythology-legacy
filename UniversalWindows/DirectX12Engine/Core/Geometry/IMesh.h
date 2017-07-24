#pragma once

namespace DirectX12Engine
{
	class IMesh
	{
	public:
		IMesh() = default;
		virtual ~IMesh() = default;

	public:
		virtual void DisposeUploadBuffers() = 0;
	};
}
