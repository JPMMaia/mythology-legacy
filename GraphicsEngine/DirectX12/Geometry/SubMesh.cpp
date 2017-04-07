#include "pch.h"
#include "SubMesh.h"

using namespace GraphicsEngine;

SubMesh::SubMesh(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation, const DirectX::BoundingBox& bounds) :
	m_indexCount(indexCount),
	m_startIndexLocation(startIndexLocation),
	m_baseVertexLocation(baseVertexLocation),
	m_bounds(bounds)
{
}

uint32_t SubMesh::IndexCount() const
{
	return m_indexCount;
}

void SubMesh::SetIndexCount(uint32_t indexCount)
{
	m_indexCount = indexCount;
}

uint32_t SubMesh::StartIndexLocation() const
{
	return m_startIndexLocation;
}

void SubMesh::SetStartIndexLocation(uint32_t startIndexLocation)
{
	m_startIndexLocation = startIndexLocation;
}

uint32_t SubMesh::BaseVertexLocation() const
{
	return m_baseVertexLocation;
}

void SubMesh::SetBaseVertexLocation(uint32_t baseVertexLocation)
{
	m_baseVertexLocation = baseVertexLocation;
}

DirectX::BoundingBox SubMesh::Bounds() const
{
	return m_bounds;
}
void SubMesh::SetBounds(const DirectX::BoundingBox& bounds)
{
	m_bounds = bounds;
}
