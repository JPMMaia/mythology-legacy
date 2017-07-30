#pragma once

#include <minwinbase.h>
#include <wrl/client.h>

#include "Core/Utilities/d3dx12.h"
#include "Core/Utilities/DirectXHelper.h"

namespace DirectX12Engine
{
	template <class T>
	class GPUAllocator
	{
	public:
		typedef T value_type;

	public:
		GPUAllocator() = default;

		template <class U>
		constexpr GPUAllocator(const GPUAllocator<U>& other) noexcept :
			m_d3dDevice(other.D3DDevice()),
			m_isConstantBuffer(other.IsConstantBuffer())
		{
		}

		explicit GPUAllocator(ID3D12Device* d3dDevice, bool isConstantBuffer) :
			m_d3dDevice(d3dDevice),
			m_isConstantBuffer(isConstantBuffer)
		{
		}

	public:
		T* allocate(std::size_t n)
		{
			if (n > std::size_t(-1) / sizeof(T))
				throw std::bad_alloc();

			return CreateUploadBuffer(n);
		}

		void deallocate(T* p, std::size_t n) noexcept
		{
			if (m_uploadBuffer != nullptr)
			{
				m_uploadBuffer->Unmap(0, nullptr);
				m_uploadBuffer = nullptr;
			}
		}

		ID3D12Device* D3DDevice() const
		{
			return m_d3dDevice;
		}
		bool IsConstantBuffer() const
		{
			return m_isConstantBuffer;
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(std::size_t index) const
		{
			return m_uploadBuffer->GetGPUVirtualAddress() + static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(index * sizeof(T));
		}

	private:
		T* CreateUploadBuffer(std::size_t size)
		{
			auto bufferSize = m_isConstantBuffer ? DX::CalculateConstantBufferByteSize(sizeof(T)) : size * sizeof(T);

			auto uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
			auto bufferDescription = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
			DX::ThrowIfFailed(
				m_d3dDevice->CreateCommittedResource(
					&uploadHeapProperties,
					D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
					&bufferDescription,
					D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(m_uploadBuffer.GetAddressOf())
				)
			);

			T* pointer;
			CD3DX12_RANGE readRange(0, 0);
			DX::ThrowIfFailed(m_uploadBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pointer)));
			ZeroMemory(pointer, bufferSize);

			return pointer;
		}

	private:
		ID3D12Device* m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadBuffer;
		bool m_isConstantBuffer;
	};

	template <class T, class U>
	bool operator==(const GPUAllocator<T>& a, const GPUAllocator<U>& b)
	{
		return false;
	}

	template <class T, class U>
	bool operator!=(const GPUAllocator<T>& a, const GPUAllocator<U>& b)
	{
		return !(a == b);
	}

	template<class DataType>
	using GPUUploadBuffer = std::vector<DataType, GPUAllocator<DataType>>;
}
