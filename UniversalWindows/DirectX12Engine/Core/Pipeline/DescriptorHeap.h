#pragma once

#include <wrl/client.h>
#include <d3d12.h>
#include "Core/DeviceResources.h"

namespace DirectX12Engine
{
	class D3DBase;

	class DescriptorHeap
	{
	public:
		explicit DescriptorHeap(const std::shared_ptr<DeviceResources>& deviceResources);

		void CreateDeviceDependentResources(size_t numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE heapType);

		INT CreateConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& description);
		INT CreateShaderResourceView(ID3D12Resource* pResource, const D3D12_SHADER_RESOURCE_VIEW_DESC& description);
		INT CreateUnorderedAccessView(ID3D12Resource* pResource, ID3D12Resource* pCounterResource, const D3D12_UNORDERED_ACCESS_VIEW_DESC& description);

		ID3D12DescriptorHeap* Get() const;
		size_t DescriptorCount() const;

		CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(INT heapOffset) const;

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
		D3D12_DESCRIPTOR_HEAP_TYPE m_type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		size_t m_capacity = 0;
		size_t m_count = 0;
	};
}
