#pragma once

#include <memory>

namespace Common 
{
	class Timer;
}

namespace GameEngine
{
	class TransformComponent;

	class IComponent
	{
	public:
		IComponent() = default;
		virtual ~IComponent() = default;

	public:
		virtual void FixedUpdate(const Common::Timer& timer) = 0;

		virtual void SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldPositionStays = false) = 0;
	};
}
