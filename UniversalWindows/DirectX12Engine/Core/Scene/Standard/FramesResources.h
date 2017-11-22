#pragma once

#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Core/DeviceResources.h"
#include "Core/Resources/GPUResource.h"

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
