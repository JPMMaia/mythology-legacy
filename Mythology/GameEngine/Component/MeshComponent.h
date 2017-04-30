#pragma once

#include "IComponent.h"
#include "TransformComponent.h"

namespace GameEngine
{
	template<typename GeometryType>
	class MeshComponent : public IComponent
	{
	public:
		MeshComponent(TransformComponent& transform, const GeometryType& geometry) :
			m_transform(transform),
			m_geometry(geometry)
		{
		}

		void FixedUpdate(const Common::Timer& timer) override
		{
		}

	private:
		TransformComponent& m_transform;
		GeometryType m_geometry;
	};
}
