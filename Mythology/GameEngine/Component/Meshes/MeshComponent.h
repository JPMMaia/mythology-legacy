#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"

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
		const GeometryType& Geometry() const
		{
			return m_geometry;
		}
		GeometryType& Geometry()
		{
			return m_geometry;
		}

	private:
		GeometryType m_geometry;
	};
}
