#pragma once

#include "GameEngine/Component/IComponent.h"

#include <memory>
#include <unordered_map>

namespace GameEngine
{
	class TransformComponent : public IComponent, public std::enable_shared_from_this<TransformComponent>
	{
	public:
		using IDType = std::size_t;
		using Vector3Type = Eigen::Vector3f;
		using Vector3CRType = const Vector3Type&;
		using QuaternionType = Eigen::Quaternion<float>;
		using QuaternionCRType = const QuaternionType&;
		using TransformType = Eigen::Transform<float, 3, Eigen::Affine>;
		
	public:
		TransformComponent();
		TransformComponent(Vector3CRType localPosition, QuaternionCRType localRotation, Vector3CRType localScaling);

	public:
		void FixedUpdate(const Common::Timer& timer);

	public:
		void Move(Vector3CRType axis, float scalar);
		void MoveLocalX(float scalar);
		void MoveLocalY(float scalar);
		void MoveLocalZ(float scalar);

		void Rotate(Vector3CRType axis, float radians);
		void RotateLocalX(float radians);
		void RotateLocalY(float radians);
		void RotateLocalZ(float radians);

		Vector3Type GetLocalX() const;
		Vector3Type GetLocalY() const;
		Vector3Type GetLocalZ() const;

		Vector3Type GetWorldX() const;
		Vector3Type GetWorldY() const;
		Vector3Type GetWorldZ() const;

	public:
		Vector3CRType GetLocalPosition() const;
		void SetLocalPosition(Vector3CRType localPosition);

		QuaternionCRType GetLocalRotation() const;
		void SetLocalRotation(QuaternionCRType localRotation);

		Vector3CRType GetLocalScale() const;
		void SetLocalScaling(Vector3CRType localScaling);

		Vector3Type GetWorldPosition() const;
		void SetWorldPosition(Vector3CRType worldPosition);

		QuaternionType GetWorldRotation() const;
		void SetWorldRotation(QuaternionCRType worldRotation);

		const std::weak_ptr<TransformComponent>& GetParent() const;
		void SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldTransformStays = false);
		void UnsetParent(bool worldTransformStays = false);

		TransformType GetLocalTransform() const;
		void SetLocalTransform(const TransformType& localTransform);

		TransformType GetWorldTransform() const;
		void SetWorldTransform(const TransformType& worldTransform);

	private:
		TransformType GetParentWorldTransform() const;
		void UpdateTransformValuesToHoldWorldTransform(const std::shared_ptr<TransformComponent>& parent, bool isNewParent);
		void InvalidateWorldTransform();
		void SetLocalTransform(const TransformType& localTransform, bool invalidateWorldTransform);

	private:
		static IDType s_count;

		IDType m_id;
		Vector3Type m_localPosition;
		QuaternionType m_localRotation;
		Vector3Type m_localScale;

		std::weak_ptr<TransformComponent> m_parent;
		std::unordered_map<IDType, std::weak_ptr<TransformComponent>> m_children;

		mutable bool m_isWorldTransformDirty;
		mutable TransformType m_worldTransform;
	};
}
