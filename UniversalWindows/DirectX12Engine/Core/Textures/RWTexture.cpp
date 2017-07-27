#include "pch.h"
#include "RWTexture.h"
#include "Core/DeviceResources.h"
#include "Core/Resources/DescriptorHeap.h"

using namespace DirectX12Engine;

RWTexture::RWTexture(const DeviceResources& deviceResources, UINT64 width, UINT64 height, DXGI_FORMAT format) :
	m_width(width),
	m_height(height),
	m_format(format)
{
	CreateResource(deviceResources, width, height, format);
}

void RWTexture::CreateRenderTargetView(const DeviceResources& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name)
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDescription = {};
	rtvDescription.Format = m_format;
	rtvDescription.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDescription.Texture2D.MipSlice = 0;

	auto heapIndex = descriptorHeap.CreateRenderTargetView(deviceResources, m_resource.Get(), &rtvDescription);
	m_cpuDescriptorHandles.emplace(name, descriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize());
	m_gpuDescriptorHandles.emplace(name, descriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize());
}
void RWTexture::CreateShaderResourceView(const DeviceResources& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDescription = {};
	srvDescription.Format = m_format;
	srvDescription.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDescription.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDescription.Texture2D.MostDetailedMip = 0;
	srvDescription.Texture2D.MipLevels = 1;

	auto heapIndex = descriptorHeap.CreateShaderResourceView(deviceResources, m_resource.Get(), &srvDescription);
	m_cpuDescriptorHandles.emplace(name, descriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize());
	m_gpuDescriptorHandles.emplace(name, descriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize());
}
void RWTexture::CreateUnorderedAccessView(const DeviceResources& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name)
{
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDescription = {};
	uavDescription.Format = m_format;
	uavDescription.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDescription.Texture2D.MipSlice = 0;

	auto heapIndex = descriptorHeap.CreateUnorderedAccessView(deviceResources, m_resource.Get(), nullptr, &uavDescription);
	m_cpuDescriptorHandles.emplace(name, CD3DX12_CPU_DESCRIPTOR_HANDLE(descriptorHeap.Get()->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize()));
	m_gpuDescriptorHandles.emplace(CD3DX12_GPU_DESCRIPTOR_HANDLE(descriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize()));
}

void RWTexture::CreateResource(const DeviceResources& deviceResources, UINT64 width, UINT64 height, DXGI_FORMAT format)
{
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
	resourceDescription.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	deviceResources.GetD3DDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDescription,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(m_resource.GetAddressOf())
	);
}
