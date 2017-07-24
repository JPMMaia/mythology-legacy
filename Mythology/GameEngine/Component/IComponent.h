#pragma once

namespace Common 
{
	class Timer;
}

namespace GameEngine
{
	class IComponent
	{
	public:
		IComponent() = default;
		virtual ~IComponent() = default;

	public:
		virtual void FixedUpdate(const Common::Timer& timer) = 0;
	};
}
