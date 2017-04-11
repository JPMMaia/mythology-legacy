#include "pch.h"
#include "ImmutableMesh.h"

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

void ImmutableMesh::Render(ID3D12GraphicsCommandList* commandList, const std::string& submeshName) const
{
	std::array<D3D12_VERTEX_BUFFER_VIEW, 1> vertexBufferViews = { m_vertexBuffer.View() };
	commandList->IASetVertexBuffers(0, 1, vertexBufferViews.data());

	std::array<D3D12_INDEX_BUFFER_VIEW, 1> indexBufferViews = { m_indexBuffer.View() };
	commandList->IASetIndexBuffer(indexBufferViews.data());

	const auto& submesh = m_submeshes.at(submeshName);
	commandList->DrawIndexedInstanced(submesh.IndexCount(), 1, submesh.StartIndexLocation(), submesh.BaseVertexLocation(), 0);
}

void ImmutableMesh::DisposeUploadBuffers()
{
	m_vertexBuffer.DisposeUploadBuffer();
	m_indexBuffer.DisposeUploadBuffer();
}

const std::string& ImmutableMesh::Name() const
{
	return m_name;
}
