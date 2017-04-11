#pragma once

namespace GraphicsEngine
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
