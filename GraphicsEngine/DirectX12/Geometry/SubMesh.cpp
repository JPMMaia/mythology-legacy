#include "pch.h"
#include "Submesh.h"

using namespace GraphicsEngine;

Submesh::Submesh(const MeshGenerator::MeshData& meshData) :
	m_indexCount(static_cast<uint32_t>(meshData.Indices.size())),
	m_startIndexLocation(0),
	m_baseVertexLocation(0),
	m_bounds(CreateBoundingBoxFromMesh(meshData.Vertices))
{
}
Submesh::Submesh(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation, const DirectX::BoundingBox& bounds) :
	m_indexCount(indexCount),
	m_startIndexLocation(startIndexLocation),
	m_baseVertexLocation(baseVertexLocation),
	m_bounds(bounds)
{
}

uint32_t Submesh::IndexCount() const
{
	return m_indexCount;
}

void Submesh::SetIndexCount(uint32_t indexCount)
{
	m_indexCount = indexCount;
}

uint32_t Submesh::StartIndexLocation() const
{
	return m_startIndexLocation;
}

void Submesh::SetStartIndexLocation(uint32_t startIndexLocation)
{
	m_startIndexLocation = startIndexLocation;
}

uint32_t Submesh::BaseVertexLocation() const
{
	return m_baseVertexLocation;
}

void Submesh::SetBaseVertexLocation(uint32_t baseVertexLocation)
{
	m_baseVertexLocation = baseVertexLocation;
}

DirectX::BoundingBox Submesh::Bounds() const
{
	return m_bounds;
}
void Submesh::SetBounds(const DirectX::BoundingBox& bounds)
{
	m_bounds = bounds;
}
