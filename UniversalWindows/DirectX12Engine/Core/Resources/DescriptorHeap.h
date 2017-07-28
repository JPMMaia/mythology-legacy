﻿#pragma once

#include <wrl/client.h>
#include <d3d12.h>

namespace DirectX12Engine
{
	class DeviceResources;

	class DescriptorHeap
	{
	public:
		DescriptorHeap() = default;

	public:
		void CreateDeviceDependentResources(const DeviceResources& deviceResources, size_t capacity, D3D12_DESCRIPTOR_HEAP_TYPE heapType, D3D12_DESCRIPTOR_HEAP_FLAGS heapFlags);
		void Clear();

		INT CreateConstantBufferView(const DeviceResources& deviceResources, const D3D12_CONSTANT_BUFFER_VIEW_DESC* description);
		INT CreateDepthStencilView(const DeviceResources& deviceResources, ID3D12Resource* resource, const D3D12_DEPTH_STENCIL_VIEW_DESC* description);
		INT CreateRenderTargetView(const DeviceResources& deviceResources, ID3D12Resource* resource, const D3D12_RENDER_TARGET_VIEW_DESC* description);
		INT CreateShaderResourceView(const DeviceResources& deviceResources, ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC* description);
		INT CreateUnorderedAccessView(const DeviceResources& deviceResources, ID3D12Resource* resource, ID3D12Resource* counterResource, const D3D12_UNORDERED_ACCESS_VIEW_DESC* description);

		ID3D12DescriptorHeap* Get() const;
		size_t GetDescriptorCount() const;

	private:
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
		D3D12_DESCRIPTOR_HEAP_TYPE m_type;
		size_t m_capacity;
		size_t m_count;
	};
}