#pragma once

#include <d3d12.h>
#include <ppltasks.h>	// For create_task

#include "Common/Helpers.h"
#include "Core/Utilities/d3dx12.h"


namespace DX
{
	void ThrowIfFailed(HRESULT hr);

	inline void UploadDataToBuffer(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, Microsoft::WRL::ComPtr<ID3D12Resource>& resource, std::vector<D3D12_SUBRESOURCE_DATA>& subresources, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer, D3D12_RESOURCE_STATES finalState)
	{
		const auto uploadBufferSize = GetRequiredIntermediateSize(resource.Get(), 0,
			static_cast<UINT>(subresources.size()));

		// Create the GPU upload buffer:
		auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
		DX::ThrowIfFailed(
			d3dDevice->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

		// Copy data from CPU to GPU:
		UpdateSubresources(commandList, resource.Get(), uploadBuffer.Get(),
			0, 0, static_cast<UINT>(subresources.size()), subresources.data());
		
		auto t1 = CD3DX12_RESOURCE_BARRIER::Transition(resource.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, finalState);
		commandList->ResourceBarrier(1, &t1);
	}

	inline Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const void* initialData, uint64_t byteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> defaultBuffer;

		// Create the actual default buffer resource:
		{
			auto defaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT);
			auto defaultBufferDescription = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
			DX::ThrowIfFailed(
				d3dDevice->CreateCommittedResource(
					&defaultHeapProperties,
					D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
					&defaultBufferDescription,
					D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON,
					nullptr,
					IID_PPV_ARGS(defaultBuffer.GetAddressOf())
				)
			);
		}

		// In order to copy CPU memory data into our default buffer, we need to create an intermediate upload buffer:
		{
			auto uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD);
			auto uploadBufferDescription = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
			DX::ThrowIfFailed(
				d3dDevice->CreateCommittedResource(
					&uploadHeapProperties,
					D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
					&uploadBufferDescription,
					D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(uploadBuffer.GetAddressOf())
				)
			);
		}

		// Schedule to copy the data to the default buffer resource:
		{
			// Describe the data we want to copy into the default buffer:
			D3D12_SUBRESOURCE_DATA subResourceData = {};
			subResourceData.pData = initialData;
			subResourceData.RowPitch = static_cast<LONG_PTR>(byteSize);
			subResourceData.SlicePitch = subResourceData.RowPitch;

			auto defaultBufferTransition1 = CD3DX12_RESOURCE_BARRIER::Transition(
				defaultBuffer.Get(),
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST
			);
			commandList->ResourceBarrier(1, &defaultBufferTransition1);

			// At a high level, the helper function UpdateSubresources will copy the CPU memory into the intermediate upload heap.
			// Then, using ID3D12CommandList::CopySubresourceRegion, the intermediate upload heap data will be copied to the default buffer.
			UpdateSubresources<1>(commandList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);

			auto defaultBufferTransition2 = CD3DX12_RESOURCE_BARRIER::Transition(
				defaultBuffer.Get(),
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ
			);
			commandList->ResourceBarrier(1, &defaultBufferTransition2);
		}

		// Note: uploadBuffer has to be kept alive after the above function calls because the command list has not been executed yet that performs the actual copy.
		// The caller can Release the uploadBuffer after it knows the copy has been executed.

		return defaultBuffer;
	}
	inline uint32_t CalculateConstantBufferByteSize(uint32_t byteSize)
	{
		return (byteSize + 255) & ~255;
	}

	// Converts a length in device-independent pixels (DIPs) to a length in physical pixels.
	inline float ConvertDipsToPixels(float dips, float dpi)
	{
		static const auto dipsPerInch = 96.0f;
		return floorf(dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
	}

	// Assign a name to the object to aid with debugging.
#if defined(_DEBUG)
	inline void SetName(ID3D12Object* pObject, LPCWSTR name)
	{
		pObject->SetName(name);
	}
#else
	inline void SetName(ID3D12Object*, LPCWSTR)
	{
	}
#endif
}

// Naming helper for ComPtr<T>.
// Assigns the name of the variable as the name of the object.
#define NAME_D3D12_OBJECT(x) DX::SetName(x.Get(), L#x)