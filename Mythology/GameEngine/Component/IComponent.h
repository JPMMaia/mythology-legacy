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

	public:
		virtual void SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldTransformStays = false) = 0;
		virtual void UnsetParent(bool worldTransformStays = false) = 0;
	};
}
