#pragma once

#include "GameEngine/Component/IComponent.h"

#include "Libraries/Eigen/Core"
#include "Libraries/Eigen/Geometry"

#include <memory>
#include <unordered_map>

namespace GameEngine
{
	class TransformComponent : public IComponent, public std::enable_shared_from_this<TransformComponent>
	{
	public:
		using IDType = std::size_t;
		using Vector3Type = Eigen::Vector3f;
		using QuaternionType = Eigen::Quaternion<float>;
		using TransformType = Eigen::Transform<float, 3, Eigen::Affine>;
		
	public:
		TransformComponent();
		TransformComponent(const Vector3Type& localPosition, const QuaternionType& localRotation, const Vector3Type& localScaling);

	public:
		void FixedUpdate(const Common::Timer& timer) override;

	public:
		const Vector3Type& GetLocalPosition() const;
		void SetLocalPosition(const Vector3Type& localPosition);

		const QuaternionType& GetLocalRotation() const;
		void SetLocalRotation(const QuaternionType& localRotation);

		const Vector3Type& GetLocalScaling() const;
		void SetLocalScaling(const Vector3Type& localScaling);

		Vector3Type GetWorldPosition() const;
		void SetWorldPosition(const Vector3Type& worldPosition);

		QuaternionType GetWorldRotation() const;
		void SetWorldRotation(const QuaternionType& worldRotation);

		const std::weak_ptr<TransformComponent>& GetParent() const;
		void SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldPositionStays = false) override;

		TransformType GetWorldTransform() const;

	private:
		TransformType CalculateLocalTransform() const;
		TransformType CalculateParentsTransform() const;

	private:
		static IDType s_count;

		IDType m_id;
		Vector3Type m_localPosition;
		QuaternionType m_localRotation;
		Vector3Type m_localScaling;

		std::weak_ptr<TransformComponent> m_parent;
		std::unordered_map<IDType, std::weak_ptr<TransformComponent>> m_children;
	};
}
