#include "pch.h"
#include "FramesResources.h"

using namespace DirectX12Engine;

FramesResources::FramesResources(const std::shared_ptr<DeviceResources>& deviceResources) :
	MaterialsBuffer(deviceResources, false, deviceResources->c_frameCount),
	PassBuffer(deviceResources, true, deviceResources->c_frameCount),
	SkinnedMeshAnimationBuffer(deviceResources, true, deviceResources->c_frameCount),
	SkinnedMeshInstancesBuffer(deviceResources, true, deviceResources->c_frameCount),
	m_deviceResources(deviceResources)
{
}

void FramesResources::Update(std::size_t frameIndex)
{
	MaterialsBuffer.Update(frameIndex);
	PassBuffer.Update(frameIndex);
	SkinnedMeshAnimationBuffer.Update(frameIndex);
	SkinnedMeshInstancesBuffer.Update(frameIndex);

	for (auto& instancesBuffer : InstancesBuffers)
		instancesBuffer.second.Update(frameIndex);
}

GPUResource<ShaderBufferTypes::InstanceData>& FramesResources::GetInstancesBuffer(const std::string& name)
{
	if (auto location = InstancesBuffers.find(name); location != InstancesBuffers.end())
		return location->second;
	
	InstancesBuffers.emplace(name, GPUResource<ShaderBufferTypes::InstanceData>(m_deviceResources, false, m_deviceResources->c_frameCount));
	return InstancesBuffers.at(name);
}
