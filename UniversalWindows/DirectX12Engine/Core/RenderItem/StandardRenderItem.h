#pragma once

#include "GraphicsEngineInterfaces/IRenderItem.h"
#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"

namespace DirectX12Engine
{
	class ImmutableMesh;

	class StandardRenderItem : public GraphicsEngine::IRenderItem
	{
	public:
		StandardRenderItem(ID3D12Device* d3dDevice);
		StandardRenderItem(ID3D12Device* d3dDevice, std::shared_ptr<ImmutableMesh> mesh, const std::string& submeshName);

		void Render(ID3D12GraphicsCommandList* commandList) const;
		void RenderNonInstanced(ID3D12GraphicsCommandList* commandList) const;

		void ReserveSpaceForInstances(std::size_t newCapacity);
		void AddInstance(const ShaderBufferTypes::InstanceData& instanceData);

	private:
		std::string m_submeshName;
		std::shared_ptr<ImmutableMesh> m_mesh;
		GPUUploadBuffer<ShaderBufferTypes::InstanceData> m_instancesGPUBuffer;
	};
}
