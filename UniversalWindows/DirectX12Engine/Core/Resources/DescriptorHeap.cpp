#include "pch.h"
#include "DescriptorHeap.h"
#include "Core/DeviceResources.h"
#include "Core/Utilities/DirectXHelper.h"

using namespace Common;
using namespace DirectX12Engine;

void DescriptorHeap::CreateDeviceDependentResources(const DeviceResources& deviceResources, size_t capacity, D3D12_DESCRIPTOR_HEAP_TYPE heapType, D3D12_DESCRIPTOR_HEAP_FLAGS heapFlags)
{
	m_capacity = capacity;
	m_count = 0;
	m_type = heapType;

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDescription = {};
	descriptorHeapDescription.NumDescriptors = static_cast<UINT>(capacity);
	descriptorHeapDescription.Type = heapType;
	descriptorHeapDescription.Flags = heapFlags;
	descriptorHeapDescription.NodeMask = 0;
	
	auto device = deviceResources.GetD3DDevice();
	DX::ThrowIfFailed(device->CreateDescriptorHeap(&descriptorHeapDescription, IID_PPV_ARGS(m_descriptorHeap.GetAddressOf())));
}
void DescriptorHeap::Clear()
{
	m_count = 0;
}

INT DescriptorHeap::CreateConstantBufferView(const DeviceResources& deviceResources, const D3D12_CONSTANT_BUFFER_VIEW_DESC* description)
{
	assert(m_type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV && "Can't create a CBV on this type of heap!");
	assert(m_count < m_capacity && "Descriptor heap is full!");

	auto heapIndex = static_cast<INT>(m_count++);
	CD3DX12_CPU_DESCRIPTOR_HANDLE destinationDescriptor(m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize());

	deviceResources.GetD3DDevice()->CreateConstantBufferView(description, destinationDescriptor);

	return heapIndex;
}
INT DescriptorHeap::CreateDepthStencilView(const DeviceResources& deviceResources, ID3D12Resource* resource, const D3D12_DEPTH_STENCIL_VIEW_DESC* description)
{
	assert(m_type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV && "Can't create a DSV on this type of heap!");
	assert(m_count < m_capacity && "Descriptor heap is full!");

	auto heapIndex = static_cast<INT>(m_count++);
	CD3DX12_CPU_DESCRIPTOR_HANDLE destinationDescriptor(m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetDsvDescriptorSize());

	deviceResources.GetD3DDevice()->CreateDepthStencilView(resource, description, destinationDescriptor);

	return heapIndex;
}
INT DescriptorHeap::CreateRenderTargetView(const DeviceResources& deviceResources, ID3D12Resource* resource, const D3D12_RENDER_TARGET_VIEW_DESC* description)
{
	assert(m_type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "Can't create a RTV on this type of heap!");
	assert(m_count < m_capacity && "Descriptor heap is full!");

	auto heapIndex = static_cast<INT>(m_count++);
	CD3DX12_CPU_DESCRIPTOR_HANDLE destinationDescriptor(m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetRtvDescriptorSize());

	deviceResources.GetD3DDevice()->CreateRenderTargetView(resource, description, destinationDescriptor);

	return heapIndex;
}
INT DescriptorHeap::CreateShaderResourceView(const DeviceResources& deviceResources, ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC* description)
{
	assert(m_type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV && "Can't create a SRV on this type of heap!");
	assert(m_count < m_capacity && "Descriptor heap is full!");

	auto heapIndex = static_cast<INT>(m_count++);
	CD3DX12_CPU_DESCRIPTOR_HANDLE destinationDescriptor(m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(), heapIndex, deviceResources.GetCbvSrvUavDescriptorSize());
	
	deviceResources.GetD3DDevice()->CreateShaderResourceView(resource, description, destinationDescriptor);

	return heapIndex;
}
INT DescriptorHeap::CreateUnorderedAccessView(const DeviceResources& deviceResources, ID3D12Resource* resource, ID3D12Resource* counterResource, const D3D12_UNORDERED_ACCESS_VIEW_DESC* description)
{
	assert(m_type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV && "Can't create an UAV on this type of heap!");
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