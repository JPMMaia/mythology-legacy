#include "pch.h"
#include "VertexBuffer.h"

using namespace DirectX12Engine;

VertexBuffer::VertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const void* vertices, size_t vertexCount, size_t vertexByteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer) :
	BaseGeometryBuffer(device, commandList, vertices, vertexCount, vertexByteSize, uploadBuffer),
	m_vertexByteStride(static_cast<uint32_t>(vertexByteSize)),
	m_view({ m_bufferGPU->GetGPUVirtualAddress(), m_bufferByteSize, m_vertexByteStride })
{
}

const D3D12_VERTEX_BUFFER_VIEW& VertexBuffer::View() const
{
	return m_view;
}
uint32_t VertexBuffer::VertexByteStride() const
{
	return m_vertexByteStride;
}
