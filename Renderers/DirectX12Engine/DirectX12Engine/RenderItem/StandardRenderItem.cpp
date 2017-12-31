#include "pch.h"
#include "StandardRenderItem.h"
#include "DirectX12Engine/Geometry/ImmutableMesh.h"

using namespace DirectX12Engine;

StandardRenderItem::StandardRenderItem(ID3D12Device& d3dDevice, const std::shared_ptr<ImmutableMesh>& mesh, const std::string& submeshName) :
	m_name(mesh->Name() + "." + submeshName),
	m_submeshName(submeshName),
	m_mesh(mesh)
{
}

void StandardRenderItem::RenderInstanced(ID3D12GraphicsCommandList& commandList, FramesResources& frameResources, std::size_t frameIndex) const
{
	// Do not render if instances buffer is empty:
	const auto& instancesBuffer = frameResources.GetInstancesBuffer(m_name);
	if (instancesBuffer.IsEmpty())
		return;

	// Bind instances' buffer:
	commandList.SetGraphicsRootShaderResourceView(0, instancesBuffer.GetGPUVirtualAddress(frameIndex, 0));

	// Set primitive topology:
	commandList.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render mesh:
	m_mesh->Render(&commandList, m_submeshName, static_cast<UINT>(instancesBuffer.GetSize()));
}
void StandardRenderItem::RenderNonInstanced(ID3D12GraphicsCommandList& commandList) const
{
	// Set primitive topology:
	commandList.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render mesh:
	m_mesh->Render(&commandList, m_submeshName, 1);
}

void StandardRenderItem::ReserveSpaceForInstances(FramesResources& frameResources, std::size_t newCapacity)
{
	auto& instancesBuffer = frameResources.GetInstancesBuffer(m_name);
	instancesBuffer.Reserve(newCapacity);
}
void StandardRenderItem::AddInstance(FramesResources& frameResources, const std::shared_ptr<RenderEngine::Instance>& instance, const ShaderBufferTypes::InstanceData& instanceData)
{
	// Add instance data to buffer:
	auto& instancesBuffer = frameResources.GetInstancesBuffer(m_name);
	auto bufferIndex = instancesBuffer.Push(instanceData);

	// Associate the buffer index with the instance:
	m_instancesIndices.emplace(bufferIndex, instance);
	instance->Index = bufferIndex;
}
void StandardRenderItem::UpdateInstance(FramesResources& frameResources, const std::shared_ptr<RenderEngine::Instance>& instance, const ShaderBufferTypes::InstanceData& instanceData)
{
	// Update the data associated with the instance:
	auto& instancesBuffer = frameResources.GetInstancesBuffer(m_name);
	auto bufferIndex = instance->Index;
	instancesBuffer.Set(bufferIndex, instanceData);
}
void StandardRenderItem::DeleteInstance(FramesResources & frameResources, std::size_t index)
{
	// Delete the data associated with the instance:
	auto bufferIndexToDelete = index;
	auto& instancesBuffer = frameResources.GetInstancesBuffer(m_name);
	auto swapped = instancesBuffer.SwapWithBackAndErase(bufferIndexToDelete);

	// If a swap did not occur:
	if (!swapped)
	{
		// Delete index:
		m_instancesIndices.erase(bufferIndexToDelete);
		return;
	}

	// Update the render info index that was on the back:
	auto bufferBackIndex = instancesBuffer.GetSize();
	auto renderInfoSwappedLocation = m_instancesIndices.find(bufferBackIndex);
	renderInfoSwappedLocation->second->Index = bufferIndexToDelete;
	m_instancesIndices[bufferIndexToDelete] = renderInfoSwappedLocation->second;
	m_instancesIndices.erase(renderInfoSwappedLocation);
}

std::size_t StandardRenderItem::GetInstanceCount(FramesResources& frameResources) const
{
	auto& instancesBuffer = frameResources.GetInstancesBuffer(m_name);
	return instancesBuffer.GetSize();
}
void StandardRenderItem::SetInstanceCount(FramesResources& frameResources, std::size_t count)
{
	auto& instancesBuffer = frameResources.GetInstancesBuffer(m_name);
	if(count != instancesBuffer.GetSize())
		instancesBuffer.Resize(count);
}
