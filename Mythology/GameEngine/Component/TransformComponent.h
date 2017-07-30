#pragma once

#include "IComponent.h"

#include "Libraries/tue/vec.hpp"
#include "Libraries/tue/quat.hpp"
#include "Libraries/tue/mat.hpp"

#include <memory>
#include <unordered_map>

namespace GameEngine
{
	class TransformComponent : public IComponent, public std::enable_shared_from_this<TransformComponent>
	{
	public:
		using IDType = std::size_t;
		using ThreeDType = tue::fvec3;
		using QuaternionType = tue::fquat;
		using MatrixType = tue::fmat4x4;

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

		const MatrixType& WorldTransform() const;

	private:
		void UpdateMatrix();

	private:
		static IDType s_count;

		IDType m_id;
		MatrixType m_worldTransform;
		bool m_dirty = true;

		ThreeDType m_translation;
		QuaternionType m_rotation;
		ThreeDType m_scale;

		std::weak_ptr<TransformComponent> m_parent;
		std::unordered_map<IDType, std::weak_ptr<TransformComponent>> m_children;
	};
}
