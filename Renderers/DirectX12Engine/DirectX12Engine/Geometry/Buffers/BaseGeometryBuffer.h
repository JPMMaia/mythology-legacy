#pragma once

#include <d3d12.h>

namespace DirectX12Engine
{
	class BaseGeometryBuffer
	{
	protected:
		BaseGeometryBuffer() = default;
		BaseGeometryBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const void* data, size_t count, size_t elementByteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);
		BaseGeometryBuffer(const BaseGeometryBuffer& rhs) = default;
		BaseGeometryBuffer& operator=(const BaseGeometryBuffer& rhs) = default;

	public:
		ID3DBlob* GetBufferCPU() const;
		ID3D12Resource* GetBufferGPU() const;
		uint32_t GetBufferByteSize() const;

	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> m_bufferCPU;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_bufferGPU;
		uint32_t m_bufferByteSize = 0;
	};
}
