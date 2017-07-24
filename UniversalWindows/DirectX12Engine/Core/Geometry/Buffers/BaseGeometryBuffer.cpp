#include "pch.h"
#include "BaseGeometryBuffer.h"
#include "Core/Utilities/DirectXHelper.h"
#include "Common/Helpers.h"

#include <d3dcompiler.h>

using namespace Common;
using namespace DirectX12Engine;

BaseGeometryBuffer::BaseGeometryBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const void* data, size_t count, size_t elementByteSize) :
	m_bufferByteSize(static_cast<uint32_t>(count * elementByteSize))
{
	// Create CPU buffer and copy vertex data:
	DX::ThrowIfFailed(
		D3DCreateBlob(m_bufferByteSize, m_bufferCPU.GetAddressOf())
	);
	CopyMemory(m_bufferCPU->GetBufferPointer(), data, m_bufferByteSize);

	// Create GPU buffer:
	m_bufferGPU = DX::CreateDefaultBuffer(device, commandList, data, m_bufferByteSize, m_uploadBuffer);
}

void BaseGeometryBuffer::DisposeUploadBuffer()
{
	m_uploadBuffer = nullptr;
}
ID3DBlob* BaseGeometryBuffer::GetBufferCPU() const
{
	return m_bufferCPU.Get();
}
ID3D12Resource* BaseGeometryBuffer::GetBufferGPU() const
{
	return m_bufferGPU.Get();
}
uint32_t BaseGeometryBuffer::GetBufferByteSize() const
{
	return m_bufferByteSize;
}
