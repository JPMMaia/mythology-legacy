#pragma once

#include "IRenderItem.h"
#include "Geometry/MeshData.h"
#include "Libraries/tue/vec.hpp"

#include <vector>

namespace OpenGLESRenderer
{
	class ColorRenderItem : public IRenderItem
	{
	public:
		explicit ColorRenderItem(const TueMeshData& meshData);

		void Draw() const;

	private:
		std::vector<tue::fvec3> m_vertices;
		std::vector<tue::fvec4> m_colors;
		std::vector<std::uint8_t> m_indices;
	};
}
