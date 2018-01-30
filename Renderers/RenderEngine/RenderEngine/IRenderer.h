#pragma once

namespace Common
{
	class Timer;
}

namespace RenderEngine
{
	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

	public:
		virtual void RecreateDeviceDependentResources() = 0;
		virtual void RecreateWindowSizeDependentResources() = 0;

	public:
		virtual bool FrameUpdate(const Common::Timer& timer) = 0;
		virtual bool Render(const Common::Timer& timer) = 0;

	public:
		virtual bool IsNextFrameAvailable() = 0;
	};
}
