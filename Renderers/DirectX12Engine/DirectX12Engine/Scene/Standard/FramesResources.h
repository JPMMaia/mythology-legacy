#pragma once

#include "DirectX12Engine/Resources/GPUAllocator.h"
#include "DirectX12Engine/Shaders/ShaderBufferTypes.h"
#include "DirectX12Engine/DeviceResources.h"
#include "DirectX12Engine/Resources/GPUResource.h"

#include <queue>
#include <unordered_map>

namespace DirectX12Engine
{
	struct FramesResources
	{
	public:
		GPUResource<ShaderBufferTypes::MaterialData> MaterialsBuffer;
		GPUResource<ShaderBufferTypes::PassData> PassBuffer;
		GPUResource<ShaderBufferTypes::SkinnedAnimationData> SkinnedMeshAnimationBuffer;
		GPUResource<ShaderBufferTypes::SkinnedMeshData> SkinnedMeshInstancesBuffer;
		std::unordered_map<std::string, GPUResource<ShaderBufferTypes::InstanceData>> InstancesBuffers;

	public:
		FramesResources(const std::shared_ptr<DeviceResources>& deviceResources);

	public:
		void Update(std::size_t frameIndex);

	public:
		GPUResource<ShaderBufferTypes::InstanceData>& GetInstancesBuffer(const std::string& name);

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
	};
}
