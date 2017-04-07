#pragma once

namespace Common 
{
	class Timer;
}

namespace GraphicsEngine
{
	class IRenderer
	{
	public:
		IRenderer() = default;
		virtual ~IRenderer() = default;

	public:
		virtual bool Render(const Common::Timer& timer) = 0;
	};
}
