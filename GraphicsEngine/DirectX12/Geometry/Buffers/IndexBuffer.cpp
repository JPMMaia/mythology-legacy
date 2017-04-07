#include "pch.h"
#include "IndexBuffer.h"

using namespace GraphicsEngine;

IndexBuffer::IndexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const void* indices, size_t indexCount, size_t indexByteSize, DXGI_FORMAT indexFormat) :
	m_indexFormat(indexFormat),
	BaseGeometryBuffer(device, commandList, indices, indexCount, indexByteSize)
{
}

DXGI_FORMAT IndexBuffer::GetIndexFormat() const
{
	return m_indexFormat;
}
