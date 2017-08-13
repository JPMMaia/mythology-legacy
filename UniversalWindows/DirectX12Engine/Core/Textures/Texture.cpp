#include "pch.h"
#include "Texture.h"
#include "Core/DeviceResources.h"
#include "Core/Resources/DescriptorHeap.h"
#include "DDSTextureLoader.h"
#include "Core/Utilities/DirectXHelper.h"

using namespace DirectX12Engine;

void Texture::CreateResource(const DeviceResources& deviceResources, UINT64 width, UINT64 height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initialResourceState, D3D12_CLEAR_VALUE* optimizedClearValue, D3D12_RESOURCE_FLAGS resourceFlags)
{
	m_format = format;

	auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC resourceDescription;
	resourceDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDescription.Alignment = 0;
	resourceDescription.Width = width;
	resourceDescription.Height = static_cast<UINT>(height);
	resourceDescription.DepthOrArraySize = 1;
	resourceDescription.MipLevels = 1;
	resourceDescription.Format = format;
	resourceDescription.SampleDesc.Count = 1;
	resourceDescription.SampleDesc.Quality = 0;
	resourceDescription.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDescription.Flags = resourceFlags;

	deviceResources.GetD3DDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDescription,
		initialResourceState,
		optimizedClearValue,
		IID_PPV_ARGS(m_resource.GetAddressOf()
		)
	);
}
void Texture::LoadTextureFromFile(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* graphicsCommandList, const std::wstring& filename, D3D12_RESOURCE_FLAGS resourceFlags, unsigned int loadFlags, DirectX::DDS_ALPHA_MODE& alphaMode, bool& isCubeMap, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
{
	std::unique_ptr<uint8_t[]> ddsData;
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;

	DX::ThrowIfFailed(
		DirectX::LoadDDSTextureFromFileEx(
			d3dDevice,
			filename.c_str(),
			0,
			resourceFlags,
			loadFlags,
			m_resource.ReleaseAndGetAddressOf(),
			ddsData,
			subresources,
			&alphaMode,
			&isCubeMap
		)
	);

	DX::UploadDataToBuffer(d3dDevice, graphicsCommandList, m_resource, subresources, uploadBuffer, D3D12_RESOURCE_STATE_COMMON);

	m_format = m_resource->GetDesc().Format;
}

void Texture::CreateDepthStencilView(const std::shared_ptr<DeviceResources>& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name)
{
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDescription;
	dsvDescription.Format = m_format;
	dsvDescription.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDescription.Flags = D3D12_DSV_FLAG_NONE;
	dsvDescription.Texture2D.MipSlice = 0;

	auto heapIndex = descriptorHeap.CreateDepthStencilView(deviceResources, m_resource.Get(), &dsvDescription);
	m_cpuDescriptorHandles.emplace(name, CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources->GetDsvDescriptorSize()));
	m_gpuDescriptorHandles.emplace(name, CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart(), heapIndex, deviceResources->GetDsvDescriptorSize()));
}
void Texture::CreateRenderTargetView(const std::shared_ptr<DeviceResources>& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name)
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDescription = {};
	rtvDescription.Format = m_format;
	rtvDescription.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDescription.Texture2D.MipSlice = 0;

	auto heapIndex = descriptorHeap.CreateRenderTargetView(deviceResources, m_resource.Get(), &rtvDescription);
	m_cpuDescriptorHandles.emplace(name, CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources->GetRtvDescriptorSize()));
	m_gpuDescriptorHandles.emplace(name, CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart(), heapIndex, deviceResources->GetRtvDescriptorSize()));
}
void Texture::CreateShaderResourceView(const std::shared_ptr<DeviceResources>& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDescription = {};
	srvDescription.Format = m_format;
	srvDescription.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDescription.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDescription.Texture2D.MostDetailedMip = 0;
	srvDescription.Texture2D.MipLevels = 1;

	auto heapIndex = descriptorHeap.CreateShaderResourceView(deviceResources, m_resource.Get(), &srvDescription);
	m_cpuDescriptorHandles.emplace(name, CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources->GetCbvSrvUavDescriptorSize()));
	m_gpuDescriptorHandles.emplace(name, CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart(), heapIndex, deviceResources->GetCbvSrvUavDescriptorSize()));
}
void Texture::CreateUnorderedAccessView(const std::shared_ptr<DeviceResources>& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDescription = {};
	uavDescription.Format = m_format;
	uavDescription.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDescription.Texture2D.MipSlice = 0;

	auto heapIndex = descriptorHeap.CreateUnorderedAccessView(deviceResources, m_resource.Get(), nullptr, &uavDescription);
	m_cpuDescriptorHandles.emplace(name, CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources->GetCbvSrvUavDescriptorSize()));
	m_gpuDescriptorHandles.emplace(name, CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart(), heapIndex, deviceResources->GetCbvSrvUavDescriptorSize()));
}

D3D12_CPU_DESCRIPTOR_HANDLE Texture::CPUDescriptorHandle(const std::string& name) const
{
	return m_cpuDescriptorHandles.at(name);
}
D3D12_GPU_DESCRIPTOR_HANDLE Texture::GPUDescriptorHandle(const std::string& name) const
{
	return m_gpuDescriptorHandles.at(name);
}

ID3D12Resource* Texture::GetResource() const
{
	return m_resource.Get();
}
