#pragma once

#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Core/DeviceResources.h"

#include <unordered_map>

namespace DirectX12Engine
{
	struct FrameResources
	{
		GPUUploadBuffer<ShaderBufferTypes::MaterialData> MaterialsBuffer;
		GPUUploadBuffer<ShaderBufferTypes::PassData> PassBuffer;
		GPUUploadBuffer<ShaderBufferTypes::SkinnedAnimationData> SkinnedMeshAnimationBuffer;
		GPUUploadBuffer<ShaderBufferTypes::SkinnedMeshData> SkinnedMeshInstancesBuffer;
		std::unordered_map<std::string, GPUUploadBuffer<ShaderBufferTypes::InstanceData>> InstancesBuffers;

		explicit FrameResources(const std::shared_ptr<DeviceResources>& deviceResources);

		GPUUploadBuffer<ShaderBufferTypes::InstanceData>& GetInstancesBuffer(const std::string& name);

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
	};
}
