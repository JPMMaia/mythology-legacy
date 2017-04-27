#pragma once

#include "IComponent.h"

#include "Libraries/tue/vec.hpp"
#include "Libraries/tue/quat.hpp"
#include "Libraries/tue/mat.hpp"

namespace GameEngine
{
	class TransformComponent : public IComponent
	{
	public:
		using ThreeDType = tue::fvec3;
		using QuaternionType = tue::fquat;
		using MatrixType = tue::fmat4x4;

	public:
		void FixedUpdate(const Common::Timer& timer) override;

	public:
		const ThreeDType& Translation() const;
		void SetTranslation(const ThreeDType& translation);

		const QuaternionType& Rotation() const;
		void SetRotation(const QuaternionType& rotation);

		const ThreeDType& Scale() const;
		void SetScale(const ThreeDType& scale);

	private:
		MatrixType m_transform;
		bool m_dirty = true;

		ThreeDType m_translation;
		QuaternionType m_rotation;
		ThreeDType m_scale;
	};
}
