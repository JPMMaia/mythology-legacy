#pragma once
#include "GameEngine/Geometry/MeshGenerator.h"

namespace GameEngine
{
	class BoxGeometry
	{
	public:
		BoxGeometry(float width, float height, float depth, std::size_t numberOfSubdivisions) :
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
			return MeshGenerator<MeshDataType>::CreateBox(m_width, m_height, m_depth, m_numberOfSubdivisions);
		}

	private:
		float m_width = 1.0f;
		float m_height = 1.0f;
		float m_depth = 1.0f;
		std::size_t m_numberOfSubdivisions = 0;
	};
}
