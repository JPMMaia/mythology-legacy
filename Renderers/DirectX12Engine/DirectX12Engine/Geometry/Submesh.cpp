#include "pch.h"
#include "Submesh.h"

using namespace DirectX12Engine;
using namespace std;

Submesh::Submesh(uint32_t indexCount, uint32_t startIndexLocation, uint32_t baseVertexLocation) :
	m_indexCount(indexCount),
	m_startIndexLocation(startIndexLocation),
	m_baseVertexLocation(baseVertexLocation)
{
}

uint32_t Submesh::GetIndexCount() const
{
	return m_indexCount;
}

void Submesh::SetIndexCount(uint32_t indexCount)
{
	m_indexCount = indexCount;
}

uint32_t Submesh::GetStartIndexLocation() const
{
	return m_startIndexLocation;
}

void Submesh::SetStartIndexLocation(uint32_t startIndexLocation)
{
	m_startIndexLocation = startIndexLocation;
}

uint32_t Submesh::GetBaseVertexLocation() const
{
	return m_baseVertexLocation;
}

void Submesh::SetBaseVertexLocation(uint32_t baseVertexLocation)
{
	m_baseVertexLocation = baseVertexLocation;
}
