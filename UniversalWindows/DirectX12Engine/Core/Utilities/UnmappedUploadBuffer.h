#pragma once

#include "Core/Utilities/d3dx12.h"
#include "Core/Utilities/DirectXHelper.h"

#include <wrl/client.h>

#include <cstddef>
#include <cstdint>
#include <memory>

namespace DirectX12Engine
{
	template<typename Type>
	class UnmappedUploadBuffer
	{
	public:
		UnmappedUploadBuffer(const std::shared_ptr<DeviceResources>& deviceResources, bool isConstantBuffer) :
			m_deviceResources(deviceResources),
			m_isConstantBuffer(isConstantBuffer)
		{
		}
		UnmappedUploadBuffer(const UnmappedUploadBuffer& rhs) = delete;
		UnmappedUploadBuffer& operator=(const UnmappedUploadBuffer& rhs) = delete;
		UnmappedUploadBuffer(UnmappedUploadBuffer&& rhs) :
			m_deviceResources(std::move(rhs.m_deviceResources)),
			m_uploadBuffer(std::move(rhs.m_uploadBuffer)),
			m_elementCount(rhs.m_elementCount),
			m_elementByteSize(rhs.m_elementByteSize),
			m_isConstantBuffer(rhs.m_isConstantBuffer)
		{
		}
		UnmappedUploadBuffer& operator=(UnmappedUploadBuffer&& rhs)
		{
			m_deviceResources = std::move(rhs.m_deviceResources);
			m_uploadBuffer = std::move(rhs.m_uploadBuffer);
			m_elementCount = rhs.m_elementCount;
			m_elementByteSize = rhs.m_elementByteSize;
			m_isConstantBuffer = rhs.m_isConstantBuffer;
		}

		void SetCapacity(std::size_t elementCount)
		{
			m_elementCount = static_cast<uint32_t>(elementCount);
			m_elementByteSize = m_isConstantBuffer ? DX::CalculateConstantBufferByteSize(sizeof(Type)) : sizeof(Type);

			auto uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
			auto bufferDescription = CD3DX12_RESOURCE_DESC::Buffer(GetBufferByteSize());

			auto d3dDevice = m_deviceResources->GetD3DDevice();
			DX::ThrowIfFailed(
				d3dDevice->CreateCommittedResource(
					&uploadHeapProperties,
					D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
					&bufferDescription,
					D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(m_uploadBuffer.GetAddressOf())
				)
			);
		}

		void CopyData(const Type* data, std::size_t destinationStart, std::size_t count)
		{
			Type* mappedData = nullptr;
			CD3DX12_RANGE readRange(0, 0);
			DX::ThrowIfFailed(m_uploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&mappedData)));

			std::memcpy(mappedData + destinationStart, data, count * sizeof(Type));

			m_uploadBuffer->Unmap(0, nullptr);
		}

		ID3D12Resource* GetResource() const
		{
			return m_uploadBuffer.Get();
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(std::uint32_t index) const
		{
			return m_uploadBuffer->GetGPUVirtualAddress() + index * m_elementByteSize;
		}

		std::uint32_t GetElementByteSize() const
		{
			return m_elementByteSize;
		}
		std::uint32_t GetBufferByteSize() const
		{
			return m_elementCount * m_elementByteSize;
		}
		std::uint32_t GetElementCount() const
		{
			return m_elementCount;
		}

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadBuffer;
		std::uint32_t m_elementCount = 0;
		std::uint32_t m_elementByteSize = 0;
		bool m_isConstantBuffer;
	};
}