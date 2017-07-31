#pragma once

#include "IComponent.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"

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

		const GeometryType* Geometry() const
		{
			return &m_geometry;
		}

	private:
		TransformComponent* m_transform = nullptr;
		GeometryType m_geometry;
	};
}
