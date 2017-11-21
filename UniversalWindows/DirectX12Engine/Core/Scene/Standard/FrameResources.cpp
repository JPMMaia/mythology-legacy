#include "pch.h"
#include "FrameResources.h"

using namespace DirectX12Engine;

FrameResources::FrameResources(const std::shared_ptr<DeviceResources>& deviceResources) :
	MaterialsBuffer(GPUAllocator<ShaderBufferTypes::MaterialData>(deviceResources->GetD3DDevice(), false)),
	PassBuffer(GPUAllocator<ShaderBufferTypes::PassData>(deviceResources->GetD3DDevice(), true)),
	SkinnedMeshAnimationBuffer(GPUAllocator<ShaderBufferTypes::SkinnedAnimationData>(deviceResources->GetD3DDevice(), true)),
	SkinnedMeshInstancesBuffer(GPUAllocator<ShaderBufferTypes::SkinnedMeshData>(deviceResources->GetD3DDevice(), true)),
	m_deviceResources(deviceResources)
{
}

GPUUploadBuffer<ShaderBufferTypes::InstanceData>& FrameResources::GetInstancesBuffer(const std::string& name)
{
	if (auto location = InstancesBuffers.find(name); location != InstancesBuffers.end())
		return location->second;

	InstancesBuffers.emplace(name, GPUAllocator<ShaderBufferTypes::InstanceData>(m_deviceResources->GetD3DDevice(), false));
	return InstancesBuffers.at(name);
}
