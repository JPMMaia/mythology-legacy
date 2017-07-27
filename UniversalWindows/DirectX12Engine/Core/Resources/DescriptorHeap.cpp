#include "pch.h"
#include "DescriptorHeap.h"
#include "Core/DeviceResources.h"
#include "Core/Utilities/DirectXHelper.h"

using namespace Common;
using namespace DirectX12Engine;

DescriptorHeap::DescriptorHeap(const DeviceResources& deviceResources, size_t capacity) :
	m_capacity(capacity),
	m_count(0)
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDescription = {};
	descriptorHeapDescription.NumDescriptors = static_cast<UINT>(capacity);
	descriptorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDescription.NodeMask = 0;

	auto device = deviceResources.GetD3DDevice();
	DX::ThrowIfFailed(device->CreateDescriptorHeap(&descriptorHeapDescription, IID_PPV_ARGS(m_descriptorHeap.GetAddressOf())));
}

INT DescriptorHeap::CreateRenderTargetView(const DeviceResources& deviceResources, ID3D12Resource* resource, const D3D12_RENDER_TARGET_VIEW_DESC* description)
{
	assert(m_count < m_capacity && "Descriptor heap is full!");

	auto heapIndex = static_cast<INT>(m_count++);
	CD3DX12_CPU_DESCRIPTOR_HANDLE destinationDescriptor(m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize());

	deviceResources.GetD3DDevice()->CreateRenderTargetView(resource, description, destinationDescriptor);

	return heapIndex;
}
INT DescriptorHeap::CreateShaderResourceView(const DeviceResources& deviceResources, ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC* description)
{
	assert(m_count < m_capacity && "Descriptor heap is full!");

	auto heapIndex = static_cast<INT>(m_count++);
	CD3DX12_CPU_DESCRIPTOR_HANDLE destinationDescriptor(m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize());

	deviceResources.GetD3DDevice()->CreateShaderResourceView(resource, description, destinationDescriptor);

	return heapIndex;
}
INT DescriptorHeap::CreateUnorderedAccessView(const DeviceResources& deviceResources, ID3D12Resource* resource, ID3D12Resource* counterResource, const D3D12_UNORDERED_ACCESS_VIEW_DESC* description)
{
	assert(m_count < m_capacity && "Descriptor heap is full!");

	auto heapIndex = static_cast<INT>(m_count++);
	CD3DX12_CPU_DESCRIPTOR_HANDLE destinationDescriptor(m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize());
	
	deviceResources.GetD3DDevice()->CreateUnorderedAccessView(resource, counterResource, description, destinationDescriptor);

	return heapIndex;
}

ID3D12DescriptorHeap* DescriptorHeap::Get() const
{
	return m_descriptorHeap.Get();
}
size_t DescriptorHeap::GetDescriptorCount() const
{
	return m_count;
}