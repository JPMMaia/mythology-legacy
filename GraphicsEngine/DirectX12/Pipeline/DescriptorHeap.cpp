#include "pch.h"
#include "DescriptorHeap.h"
#include "Common/Helpers.h"
#include "DirectX12/Utilities/DirectXHelper.h"

using namespace Common;
using namespace GraphicsEngine;

DescriptorHeap::DescriptorHeap(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
}

void DescriptorHeap::CreateDeviceDependentResources(size_t numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE heapType)
{
	m_capacity = numDescriptors;
	m_type = heapType;

	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDescription = {};
	descriptorHeapDescription.NumDescriptors = static_cast<UINT>(numDescriptors);
	descriptorHeapDescription.Type = heapType;
	descriptorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptorHeapDescription.NodeMask = 0;

	auto device = m_deviceResources->GetD3DDevice();
	Helpers::ThrowIfFailed(device->CreateDescriptorHeap(&descriptorHeapDescription, IID_PPV_ARGS(m_descriptorHeap.GetAddressOf())));
	NAME_D3D12_OBJECT(m_descriptorHeap);
}

INT DescriptorHeap::CreateConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& description)
{
	assert(m_type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV && "Descriptor heap type must be CRV_SRV_UAV!");
	assert(m_count < m_capacity && "Descriptor heap is full!");

	auto heapIndex = static_cast<INT>(m_count++);
	CD3DX12_CPU_DESCRIPTOR_HANDLE destinationDescriptor(m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(), heapIndex, m_deviceResources->GetCbvSrvUavDescriptorSize());

	auto device = m_deviceResources->GetD3DDevice();
	device->CreateConstantBufferView(&description, destinationDescriptor);

	return heapIndex;
}
INT DescriptorHeap::CreateShaderResourceView(ID3D12Resource* pResource, const D3D12_SHADER_RESOURCE_VIEW_DESC& description)
{
	assert(m_type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV && "Descriptor heap type must be CRV_SRV_UAV!");
	assert(m_count < m_capacity && "Descriptor heap is full!");

	auto heapIndex = static_cast<INT>(m_count++);
	CD3DX12_CPU_DESCRIPTOR_HANDLE destinationDescriptor(m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(), heapIndex, m_deviceResources->GetCbvSrvUavDescriptorSize());

	auto device = m_deviceResources->GetD3DDevice();
	device->CreateShaderResourceView(pResource, &description, destinationDescriptor);

	return heapIndex;
}
INT DescriptorHeap::CreateUnorderedAccessView(ID3D12Resource* pResource, ID3D12Resource *pCounterResource, const D3D12_UNORDERED_ACCESS_VIEW_DESC& description)
{
	assert(m_type == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV && "Descriptor heap type must be CRV_SRV_UAV!");
	assert(m_count < m_capacity && "Descriptor heap is full!");

	auto heapIndex = static_cast<INT>(m_count++);
	CD3DX12_CPU_DESCRIPTOR_HANDLE destinationDescriptor(m_descriptorHeap->GetCPUDescriptorHandleForHeapStart(), heapIndex, m_deviceResources->GetCbvSrvUavDescriptorSize());

	auto device = m_deviceResources->GetD3DDevice();
	device->CreateUnorderedAccessView(pResource, pCounterResource, &description, destinationDescriptor);

	return heapIndex;
}

ID3D12DescriptorHeap* DescriptorHeap::Get() const
{
	return m_descriptorHeap.Get();
}

size_t DescriptorHeap::DescriptorCount() const
{
	return m_count;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGPUDescriptorHandle(INT heapOffset) const
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descriptorHeap->GetGPUDescriptorHandleForHeapStart(), heapOffset, m_deviceResources->GetCbvSrvUavDescriptorSize());
}
