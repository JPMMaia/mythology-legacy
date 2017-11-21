#pragma once

#include "IRenderItem.h"
#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Core/Scene/Standard/FrameResources.h"

namespace DirectX12Engine
{
	class ImmutableMesh;

	class StandardRenderItem : public GraphicsEngine::IRenderItem
	{
	public:
		StandardRenderItem(ID3D12Device& d3dDevice, const std::shared_ptr<ImmutableMesh>& mesh, const std::string& submeshName);

		void RenderInstanced(ID3D12GraphicsCommandList& commandList, const FrameResources& frameResources) const;
		void RenderNonInstanced(ID3D12GraphicsCommandList& commandList) const;

		void ReserveSpaceForInstances(FrameResources& frameResources, std::size_t newCapacity);
		void AddInstance(FrameResources& frameResources, const ShaderBufferTypes::InstanceData& instanceData);
		void UpdateInstance(FrameResources& frameResources, std::size_t index, const ShaderBufferTypes::InstanceData& instanceData);
		std::size_t GetInstanceCount(FrameResources& frameResources) const;
		void SetInstanceCount(FrameResources& frameResources, std::size_t count);

	private:
		std::string m_name;
		std::string m_submeshName;
		std::shared_ptr<ImmutableMesh> m_mesh;
	};
}
