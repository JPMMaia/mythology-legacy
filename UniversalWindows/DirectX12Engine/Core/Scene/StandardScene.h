﻿#pragma once

#include "IScene.h"
#include "Core/DeviceResources.h"
#include "Core/Command/CommandListManager.h"
#include "Core/RenderItem/StandardRenderItem.h"
#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Core/Textures/Texture.h"
#include "Core/Resources/DescriptorHeap.h"
#include "Mythology/MythologyGame.h"

#include <unordered_map>
#include "GameEngine/Geometry/EigenGeometry.h"
#include "Core/Geometry/Buffers/VertexBuffer.h"
#include "Core/Geometry/Buffers/IndexBuffer.h"

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
		VertexBuffer CreateVertexBuffer(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const GameEngine::EigenMeshData& meshData);
		IndexBuffer CreateIndexBuffer(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const GameEngine::EigenMeshData& meshData);

		template<class MeshType>
		void CreateRenderItems(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList);

		void CreateMaterial(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const GameEngine::StandardMaterial& material);
		void CreateTexture(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::wstring& path, bool isColorData);

		void UpdatePassBuffer();
		void UpdateSkinnedBuffers();
		void UpdateInstancesBuffers();

		template<class MeshType>
		void UpdateInstancesBuffer();

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		CommandListManager& m_commandListManager;
		std::size_t m_commandListIndex = 0;

		std::deque<Microsoft::WRL::ComPtr<ID3D12Resource>> m_temporaryUploadBuffers;
		GPUUploadBuffer<ShaderBufferTypes::MaterialData> m_materialsGPUBuffer;
		GPUUploadBuffer<ShaderBufferTypes::PassData> m_passGPUBuffer;

		std::unique_ptr<StandardRenderItem> m_renderRectangle;
		std::deque<std::unique_ptr<StandardRenderItem>> m_renderItems;
		std::unordered_map<RenderLayer, std::deque<StandardRenderItem*>> m_renderItemsPerLayer;
		std::unordered_map<std::string, StandardRenderItem*> m_renderItemsPerGeometry;

		std::unordered_map<std::wstring, Texture> m_textures;
		DescriptorHeap m_texturesDescriptorHeap;

		std::unordered_map<std::string, std::uint32_t> m_materialIndices;
		std::unordered_map<std::wstring, std::uint32_t> m_textureIndices;

		std::shared_ptr<Mythology::MythologyGame> m_game;
		
		GPUUploadBuffer<ShaderBufferTypes::SkinnedData> m_skinnedGPUBuffer;
	};
}
