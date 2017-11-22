#pragma once

#include "IRenderItem.h"
#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Core/Scene/Standard/FramesResources.h"

namespace DirectX12Engine
{
	class ImmutableMesh;

	class StandardRenderItem : public GraphicsEngine::IRenderItem
	{
	public:
		StandardRenderItem(ID3D12Device& d3dDevice, const std::shared_ptr<ImmutableMesh>& mesh, const std::string& submeshName);

		void RenderInstanced(ID3D12GraphicsCommandList& commandList, FramesResources& frameResources, std::size_t frameIndex) const;
		void RenderNonInstanced(ID3D12GraphicsCommandList& commandList) const;

		void ReserveSpaceForInstances(FramesResources& frameResources, std::size_t newCapacity);
		void AddInstance(FramesResources& frameResources, const ShaderBufferTypes::InstanceData& instanceData);
		void UpdateInstance(FramesResources& frameResources, std::size_t index, const ShaderBufferTypes::InstanceData& instanceData);
		std::size_t GetInstanceCount(FramesResources& frameResources) const;
		void SetInstanceCount(FramesResources& frameResources, std::size_t count);

	private:
		std::string m_name;
		std::string m_submeshName;
		std::shared_ptr<ImmutableMesh> m_mesh;
	};
}
