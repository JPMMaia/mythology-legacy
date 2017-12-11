#pragma once

#include "IRenderItem.h"
#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Core/Scene/Standard/FramesResources.h"
#include "GameEngine/Component/Meshes/RenderInfo.h"

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
		void AddInstance(FramesResources& frameResources, const std::shared_ptr<GameEngine::RenderInfo>& renderInfo, const ShaderBufferTypes::InstanceData& instanceData);
		void UpdateInstance(FramesResources& frameResources, const GameEngine::RenderInfo& index, const ShaderBufferTypes::InstanceData& instanceData);
		void DeleteInstance(FramesResources& frameResources, const GameEngine::RenderInfo& index);
		std::size_t GetInstanceCount(FramesResources& frameResources) const;
		void SetInstanceCount(FramesResources& frameResources, std::size_t count);

	private:
		std::string m_name;
		std::string m_submeshName;
		std::shared_ptr<ImmutableMesh> m_mesh;
		std::unordered_map<std::size_t, std::shared_ptr<GameEngine::RenderInfo>> m_instancesIndices;
	};
}
