#pragma once

#include "Core/Utilities/d3dx12.h"

#include <d3d12.h>
#include <wrl/Client.h>
#include <unordered_map>

namespace DirectX12Engine
{
	class DeviceResources;
	class DescriptorHeap;

	class RWTexture
	{
	public:
		RWTexture() = default;

	public:
		void CreateWindowSizeDependentResources(const DeviceResources& deviceResources, UINT64 width, UINT64 height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initialResourceState, D3D12_CLEAR_VALUE* optimizedClearValue, D3D12_RESOURCE_FLAGS resourceFlags);

		void CreateDepthStencilView(const DeviceResources& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);
		void CreateRenderTargetView(const DeviceResources& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);
		void CreateShaderResourceView(const DeviceResources& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);
		void CreateUnorderedAccessView(const DeviceResources& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);

		D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHandle(const std::string& name) const;
		D3D12_GPU_DESCRIPTOR_HANDLE GPUDescriptorHandle(const std::string& name) const;

		ID3D12Resource* GetResource() const;

	private:
		void CreateResource(const DeviceResources& deviceResources, UINT64 width, UINT64 height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initialResourceState, D3D12_CLEAR_VALUE* optimizedClearValue, D3D12_RESOURCE_FLAGS resourceFlags);

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
		UINT64 m_width;
		UINT64 m_height;
		DXGI_FORMAT m_format;
		std::unordered_map<std::string, CD3DX12_CPU_DESCRIPTOR_HANDLE> m_cpuDescriptorHandles;
		std::unordered_map<std::string, CD3DX12_GPU_DESCRIPTOR_HANDLE> m_gpuDescriptorHandles;
	};
}
