#pragma once

#include "GameEngine/Component/IComponent.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"

namespace GameEngine
{
	class BaseComponent : public IComponent
	{
	public:
		using Vector3Type = Eigen::Vector3f;
		using QuaternionType = Eigen::Quaternion<float>;

	public:
		BaseComponent() = default;

	public:
		void FixedUpdate(const Common::Timer& timer) override;

		void SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldPositionStays = false) override;

		const TransformComponent& Transform() const;
		TransformComponent& Transform();

	protected:
		TransformComponent m_transform;
	};
}
