#include "pch.h"
#include "IndexBuffer.h"

using namespace DirectX12Engine;

IndexBuffer::IndexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const void* indices, size_t indexCount, size_t indexByteSize, DXGI_FORMAT indexFormat, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer) :
	BaseGeometryBuffer(device, commandList, indices, indexCount, indexByteSize, uploadBuffer),
	m_indexFormat(indexFormat),
	m_view({ m_bufferGPU->GetGPUVirtualAddress(), m_bufferByteSize, m_indexFormat })
{
}

const D3D12_INDEX_BUFFER_VIEW& IndexBuffer::View() const
{
	return m_view;
}
DXGI_FORMAT IndexBuffer::GetIndexFormat() const
{
	return m_indexFormat;
}
