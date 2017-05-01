#pragma once

#include "IComponent.h"
#include "TransformComponent.h"

namespace GameEngine
{
	template<typename GeometryType>
	class MeshComponent : public IComponent
	{
	public:
		MeshComponent() = default;
		MeshComponent(TransformComponent* transform, const GeometryType& geometry) :
			m_transform(transform),
			m_geometry(geometry)
		{
		}

	public:
		void FixedUpdate(const Common::Timer& timer) override
		{
		}

	private:
		TransformComponent* m_transform = nullptr;
		GeometryType m_geometry;
	};
}
