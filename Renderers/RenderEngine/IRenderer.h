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
		virtual void CreateDeviceDependentResources() = 0;
		virtual void CreateWindowSizeDependentResources() = 0;
		virtual bool FrameUpdate(const Common::Timer& timer) = 0;
		virtual bool Render(const Common::Timer& timer) = 0;
	};
}