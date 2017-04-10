#include "pch.h"
#include "IndexBuffer.h"

using namespace GraphicsEngine;

IndexBuffer::IndexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const void* indices, size_t indexCount, size_t indexByteSize, DXGI_FORMAT indexFormat) :
	BaseGeometryBuffer(device, commandList, indices, indexCount, indexByteSize),
	m_indexFormat(indexFormat),
	m_view({ m_bufferGPU->GetGPUVirtualAddress(), m_bufferByteSize, m_indexFormat })
{
}

DXGI_FORMAT IndexBuffer::GetIndexFormat() const
{
	return m_indexFormat;
}
