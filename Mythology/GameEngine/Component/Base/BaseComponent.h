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
		BaseComponent();
		explicit BaseComponent(const std::shared_ptr<TransformComponent>& transform);

	public:
		void FixedUpdate(const Common::Timer& timer) override;

	public:
		void SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldTransformStays = false) override;
		void UnsetParent(bool worldTransformStays = false) override;

	public:
		const TransformComponent& GetTransform() const;
		TransformComponent& GetTransform();

		const std::shared_ptr<TransformComponent>& GetSharedTransform() const;

	protected:
		void SetTransform(const std::shared_ptr<TransformComponent>& transform);

	private:
		std::shared_ptr<TransformComponent> m_transform;
	};
}
