#pragma once

#include "IComponent.h"
#include "Base/BaseComponent.h"

namespace GameEngine
{
	template<typename GeometryType>
	class MeshComponent : public BaseComponent
	{
	public:
		MeshComponent() = default;
		explicit MeshComponent(const GeometryType& geometry) :
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
		GeometryType m_geometry;
	};
}
