#pragma once

#include "IRenderItem.h"
#include "DirectX12Engine/Resources/GPUAllocator.h"
#include "DirectX12Engine/Shaders/ShaderBufferTypes.h"
#include "DirectX12Engine/Scene/Standard/FramesResources.h"
#include "RenderEngine/Instances/Instance.h"

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
		void AddInstance(FramesResources& frameResources, const std::shared_ptr<RenderEngine::Instance>& instance, const ShaderBufferTypes::InstanceData& instanceData);
		void UpdateInstance(FramesResources& frameResources, const RenderEngine::Instance& instance, const ShaderBufferTypes::InstanceData& instanceData);
		void DeleteInstance(FramesResources& frameResources, const RenderEngine::Instance& instance);
		std::size_t GetInstanceCount(FramesResources& frameResources) const;
		void SetInstanceCount(FramesResources& frameResources, std::size_t count);

	private:
		std::string m_name;
		std::string m_submeshName;
		std::shared_ptr<ImmutableMesh> m_mesh;
		std::unordered_map<std::size_t, std::shared_ptr<RenderEngine::Instance>> m_instancesIndices;
	};
}
