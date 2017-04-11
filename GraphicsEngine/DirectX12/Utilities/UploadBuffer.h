#pragma once

#include "GraphicsEngine/DirectX12/Utilities/d3dx12.h"
#include "DirectXHelper.h"
#include <minwinbase.h>

namespace GraphicsEngine
{
	template<typename Type>
	class UploadBuffer
	{
	public:
		UploadBuffer(ID3D12Device* d3dDevice, size_t elementCount, bool isConstantBuffer) :
			m_elementCount(static_cast<uint32_t>(elementCount)),
			m_elementByteSize(isConstantBuffer ? DX::CalculateConstantBufferByteSize(sizeof(Type)) : sizeof(Type)),
			m_isConstantBuffer(isConstantBuffer)
		{
			auto uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
			auto bufferDescription = CD3DX12_RESOURCE_DESC::Buffer(GetBufferByteSize());
			Common::Helpers::ThrowIfFailed(
				d3dDevice->CreateCommittedResource(
					&uploadHeapProperties,
					D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
					&bufferDescription,
					D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(m_uploadBuffer.GetAddressOf())
				)
			);

			CD3DX12_RANGE readRange(0, 0);
			Common::Helpers::ThrowIfFailed(m_uploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedData)));
			ZeroMemory(m_mappedData, GetBufferByteSize());

			// We do not need to unmap until we are done with the resource.
			// However, we must not write to the resource while it is in use by the GPU (so we must use synchronization techniques).
		}
		UploadBuffer(const UploadBuffer& rhs) = delete;
		UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
		~UploadBuffer()
		{
			if (m_uploadBuffer != nullptr)
			{
				m_uploadBuffer->Unmap(0, nullptr);
				m_uploadBuffer = nullptr;
			}

			m_mappedData = nullptr;
		}

		void CopyData(int elementIndex, const Type& data) const
		{
			assert(elementIndex < static_cast<int>(m_elementCount));

			memcpy_s(&m_mappedData[elementIndex * m_elementByteSize], sizeof(Type), &data, sizeof(Type));
		}

		ID3D12Resource* GetResource() const
		{
			return m_uploadBuffer.Get();
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(uint32_t index) const
		{
			return m_uploadBuffer->GetGPUVirtualAddress() + index * m_elementByteSize;
		}

		uint32_t GetElementByteSize() const
		{
			return m_elementByteSize;
		}
		uint32_t GetBufferByteSize() const
		{
			return m_elementCount * m_elementByteSize;
		}
		uint32_t GetElementCount() const
		{
			return m_elementCount;
		}

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadBuffer;
		BYTE* m_mappedData = nullptr;

		uint32_t m_elementCount = 0;
		uint32_t m_elementByteSize = 0;
		bool m_isConstantBuffer = false;
	};
}
