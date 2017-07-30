#pragma once

#include "IComponent.h"

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
		using ThreeDType = Eigen::Vector3f;
		using QuaternionType = Eigen::Quaternion<float>;
		using TransformType = Eigen::Transform<float, 3, Eigen::Affine>;
		
	public:
		TransformComponent();

	public:
		void FixedUpdate(const Common::Timer& timer) override;

	public:
		const ThreeDType& Translation() const;
		void SetTranslation(const ThreeDType& translation);

		const QuaternionType& Rotation() const;
		void SetRotation(const QuaternionType& rotation);

		const ThreeDType& Scale() const;
		void SetScale(const ThreeDType& scale);

		const std::weak_ptr<TransformComponent>& GetParent() const;
		void SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldPositionStays = false);

		const TransformType& WorldTransform() const;

	private:
		TransformType CalculateLocalTransform() const;
		void UpdateMatrix();

	private:
		static IDType s_count;

		IDType m_id;
		TransformType m_worldTransform;
		bool m_dirty = true;

		ThreeDType m_translation;
		QuaternionType m_rotation;
		ThreeDType m_scale;

		std::weak_ptr<TransformComponent> m_parent;
		std::unordered_map<IDType, std::weak_ptr<TransformComponent>> m_children;
	};
}
