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
		RWTexture(const DeviceResources& deviceResources, UINT64 width, UINT64 height, DXGI_FORMAT format);

	public:
		void CreateRenderTargetView(const DeviceResources& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);
		void CreateShaderResourceView(const DeviceResources& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);
		void CreateUnorderedAccessView(const DeviceResources& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);

	private:
		void CreateResource(const DeviceResources& deviceResources, UINT64 width, UINT64 height, DXGI_FORMAT format);

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
		UINT64 m_width;
		UINT64 m_height;
		DXGI_FORMAT m_format;
		std::unordered_map<std::string, CD3DX12_CPU_DESCRIPTOR_HANDLE> m_cpuDescriptorHandles;
		std::unordered_map<std::string, CD3DX12_GPU_DESCRIPTOR_HANDLE> m_gpuDescriptorHandles;
	};
}
