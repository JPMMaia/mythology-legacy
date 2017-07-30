#include "pch.h"
#include "StandardRenderItem.h"
#include "Core/Geometry/ImmutableMesh.h"

using namespace DirectX12Engine;

StandardRenderItem::StandardRenderItem(ID3D12Device* d3dDevice) :
	m_instancesGPUBuffer(GPUAllocator<ShaderBufferTypes::InstanceData>(d3dDevice, false))
{
}
StandardRenderItem::StandardRenderItem(ID3D12Device* d3dDevice, std::shared_ptr<ImmutableMesh> mesh, const std::string& submeshName) :
	m_submeshName(submeshName),
	m_mesh(mesh),
	m_instancesGPUBuffer(GPUAllocator<ShaderBufferTypes::InstanceData>(d3dDevice, false))
{
}

void StandardRenderItem::Render(ID3D12GraphicsCommandList* commandList) const
{
	// Do not render if instances buffer is empty:
	if (m_instancesGPUBuffer.empty())
		return;

	// Bind instances' buffer:
	commandList->SetGraphicsRootShaderResourceView(0, m_instancesGPUBuffer.get_allocator().GetGPUVirtualAddress(0));

	// Set primitive topology:
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render mesh:
	m_mesh->Render(commandList, m_submeshName, static_cast<UINT>(m_instancesGPUBuffer.size()));
}
void StandardRenderItem::RenderNonInstanced(ID3D12GraphicsCommandList* commandList) const
{
	// Set primitive topology:
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render mesh:
	m_mesh->Render(commandList, m_submeshName, 1);
}

void StandardRenderItem::ReserveSpaceForInstances(std::size_t newCapacity)
{
	m_instancesGPUBuffer.reserve(newCapacity);
}
void StandardRenderItem::AddInstance(const ShaderBufferTypes::InstanceData& instanceData)
{
	m_instancesGPUBuffer.push_back(instanceData);
}
