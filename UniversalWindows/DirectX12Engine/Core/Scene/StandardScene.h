#pragma once

#include "IScene.h"
#include "Core/DeviceResources.h"
#include "Core/Command/CommandListManager.h"
#include "Core/RenderItem/StandardRenderItem.h"
#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Core/Textures/Texture.h"
#include "Core/Resources/DescriptorHeap.h"
#include "Mythology/MythologyGame.h"
#include "GameEngine/Component/Meshes/SkinnedMeshComponent.h"
#include "GameEngine/Geometry/EigenGeometry.h"
#include "Core/Geometry/Buffers/VertexBuffer.h"
#include "Core/Geometry/Buffers/IndexBuffer.h"
#include "Core/Scene/Standard/FramesResources.h"
#include "GameEngine/Events/MaterialEventsQueue.h"
#include "GameEngine/Events/MeshEventsQueue.h"
#include "GameEngine/Events/InstanceEventsQueue.h"

#include <unordered_map>

namespace DirectX12Engine
{
	enum class RenderLayer;

	class StandardScene : public IScene
	{
	public:
		StandardScene(const std::shared_ptr<DeviceResources>& deviceResources, CommandListManager& commandListManager, const std::shared_ptr<Mythology::MythologyGame>& game);
		~StandardScene();

		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;
		
		void SaveState() override;
		void LoadState() override;

		void ProcessInput() override;
		void FrameUpdate(const Common::Timer& timer) override;

		bool Render(const Common::Timer& timer, RenderLayer renderLayer) override;

	private:
		VertexBuffer CreateVertexBuffer(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const GameEngine::EigenMeshData& meshData, bool isSkinned);
		IndexBuffer CreateIndexBuffer(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const GameEngine::EigenMeshData& meshData);

		template<class MeshType>
		void CreateRenderItems(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList);

		void CreateRenderItem(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::string& meshName, const GameEngine::EigenMeshData& meshData);
		void CreateMaterial(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const GameEngine::StandardMaterial& material);
		void CreateTextureFromFile(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::wstring& path, bool isColorData);
		void CreateEmptyTexture(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::wstring& name, bool isColorData);

		void UpdatePassBuffer();
		void UpdateSkinnedAnimationBuffers();
		void UpdateInstancesBuffers();

		template<class MeshType>
		void UpdateInstancesBuffer();

	private:
		void OnMaterialCreated(GameEngine::MaterialEventsQueue::EventArg eventArgs);
		void OnMaterialDeleted(GameEngine::MaterialEventsQueue::EventArg eventArgs);
		
		void OnMeshesCreated(GameEngine::MeshEventsQueue::EventArg eventArgs);
		void OnMeshesDeleted(GameEngine::MeshEventsQueue::EventArg eventArgs);

		void OnInstancesCreated(GameEngine::InstanceEventsQueue::EventArg eventArgs);
		void OnInstancesUpdated(GameEngine::InstanceEventsQueue::EventArg eventArgs);
		void OnInstancesDeleted(GameEngine::InstanceEventsQueue::EventArg eventArgs);

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		CommandListManager& m_commandListManager;
		std::size_t m_commandListIndex = 0;

		std::deque<Microsoft::WRL::ComPtr<ID3D12Resource>> m_temporaryUploadBuffers;
		FramesResources m_framesResources;

		std::unique_ptr<StandardRenderItem> m_renderRectangle;
		std::deque<std::unique_ptr<StandardRenderItem>> m_renderItems;
		std::unordered_map<RenderLayer, std::deque<StandardRenderItem*>> m_renderItemsPerLayer;
		std::unordered_map<std::string, StandardRenderItem*> m_renderItemsPerGeometry;

		std::unordered_map<std::wstring, Texture> m_textures;
		DescriptorHeap m_texturesDescriptorHeap;

		std::unordered_map<std::string, std::uint32_t> m_materialIndices;
		std::unordered_map<std::wstring, std::uint32_t> m_textureIndices;

		std::shared_ptr<Mythology::MythologyGame> m_game;
		
		std::unordered_map<std::string, std::deque<StandardRenderItem*>> m_renderItemsPerSkinnedMesh;
		std::unordered_map<StandardRenderItem*, std::uint32_t> m_skinnedRenderItemsMaterialIndices;

		std::size_t m_currentFrameIndex = 0;
	};
}
