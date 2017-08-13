#pragma once

#include "GameEngine/Geometry/MeshGenerator.h"

#include <cstddef>

namespace GameEngine
{
	class RectangleGeometry
	{
	public:
		RectangleGeometry() = default;
		RectangleGeometry(float originX, float originY, float width, float height, float depth, std::size_t numberOfSubdivisions) :
			m_originX(originX),
			m_originY(originY),
			m_width(width),
			m_height(height),
			m_depth(depth),
			m_numberOfSubdivisions(numberOfSubdivisions)
		{
		}

	public:
		template<typename MeshDataType>
		MeshDataType GenerateMeshData() const
		{
			return MeshGenerator<MeshDataType>::CreateRectangle(m_originX, m_originY, m_width, m_height, m_depth, m_numberOfSubdivisions);
		}

	private:
		float m_originX = 0.0f;
		float m_originY = 0.0f;
		float m_width = 1.0f;
		float m_height = 1.0f;
		float m_depth = 1.0f;
		std::size_t m_numberOfSubdivisions = 0;
	};
}
