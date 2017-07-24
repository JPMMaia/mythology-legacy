#include "ColorRenderItem.h"

using namespace OpenGLESRenderer;

ColorRenderItem::ColorRenderItem(const TueMeshData& meshData)
{
	// Vertices:
	{
		const auto& vertices = meshData.Vertices;
		m_vertices.resize(vertices.size());
		for (std::size_t i = 0; i < vertices.size(); ++i)
			m_vertices[i] = vertices[i].Position;
	}

	// Colors
	{
		m_colors.resize(meshData.Vertices.size());
		for (std::size_t i = 0; i < m_colors.size(); ++i)
			m_colors[i] = { 0.0f, 0.0f, 1.0f, 1.0f };
	}

	// Indices
	{
		const auto& indices = meshData.Indices;
		m_indices.resize(indices.size());
		for (std::size_t i = 0; i < indices.size(); ++i)
			m_indices[i] = indices[i];
	}
}

void ColorRenderItem::Draw() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glFrontFace(GL_CW);
	glVertexPointer(3, GL_FLOAT, 0, m_vertices.data());
	glColorPointer(4, GL_FLOAT, 0, m_colors.data());
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_BYTE, m_indices.data());

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
