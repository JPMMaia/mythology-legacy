#include "pch.h"
#include "DirectX12/Geometry/ImmutableMesh.h"

using namespace GraphicsEngine;

ImmutableMesh::ImmutableMesh(const std::string& name, VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer) :
m_name(name),
m_vertexBuffer(std::move(vertexBuffer)),
m_indexBuffer(std::move(indexBuffer))
{
}

void ImmutableMesh::AddSubmesh(const std::string& name, const Submesh& submesh)
{
	m_submeshes.emplace(name, submesh);
}

const std::string& ImmutableMesh::Name() const
{
	return m_name;
}
