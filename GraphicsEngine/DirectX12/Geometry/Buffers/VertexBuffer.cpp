#include "pch.h"
#include "VertexBuffer.h"

using namespace GraphicsEngine;

VertexBuffer::VertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const void* vertices, size_t vertexCount, size_t vertexByteSize) :
	m_vertexByteStride(static_cast<uint32_t>(vertexByteSize)),
	BaseGeometryBuffer(device, commandList, vertices, vertexCount, vertexByteSize)
{
}

uint32_t VertexBuffer::GetVertexByteStride() const
{
	return m_vertexByteStride;
}
