#pragma once

#include "RenderEngine/IScene.h"
#include "DirectX12Engine/DeviceResources.h"
#include "DirectX12Engine/Command/CommandListManager.h"
#include "DirectX12Engine/RenderItem/StandardRenderItem.h"
#include "DirectX12Engine/Resources/GPUAllocator.h"
#include "DirectX12Engine/Shaders/ShaderBufferTypes.h"
#include "DirectX12Engine/Textures/Texture.h"
#include "DirectX12Engine/Resources/DescriptorHeap.h"
#include "DirectX12Engine/Geometry/Buffers/VertexBuffer.h"
#include "DirectX12Engine/Geometry/Buffers/IndexBuffer.h"
#include "DirectX12Engine/Scene/Standard/FramesResources.h"
#include "RenderLayers.h"

#include <unordered_map>

namespace DirectX12Engine
{
	class StandardScene : public RenderEngine::IScene
	{
	public:
		StandardScene(const std::shared_ptr<DeviceResources>& deviceResources, CommandListManager& commandListManager);

	public:
		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;

	public:
		void FrameUpdate(const Common::Timer& timer) override;
		bool Render(const Common::Timer& timer, RenderEngine::RenderLayer renderLayer) override;

	public:
		void CreateMesh(const std::string& name, const RenderEngine::MeshData& meshData) override;
		void DeleteMesh(const std::string& name) override;

		void CreateMaterial(const std::string& name, const RenderEngine::Material& material) override;
		void UpdateMaterial(const std::string& name, const RenderEngine::Material& material) override;
		void DeleteMaterial(const std::string& name) override;

		void CreateInstance(const std::string& meshName, const std::shared_ptr<RenderEngine::Instance>& instance) override;
		void UpdateInstance(const std::string& meshName, const RenderEngine::Instance& instance) override;
		void DeleteInstance(const std::string& meshName, const RenderEngine::Instance& instance) override;

	private:
		template<class MeshType>
		VertexBuffer CreateVertexBuffer(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const MeshType& meshData);

		template<class MeshType>
		IndexBuffer CreateIndexBuffer(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const MeshType& meshData);

		template<class MeshType>
		void CreateRenderItem(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::string& meshName, const MeshType& meshData);

	private:
		void CreateMaterial(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const RenderEngine::Material& material);
		std::uint32_t CreateTextureFromFile(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::wstring& path, bool isColorData);
		std::uint32_t CreateEmptyTexture(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::wstring& name, bool isColorData);

	private:
		void UpdatePassBuffer();
		void UpdateSkinnedAnimationBuffers();
		void UpdateInstancesBuffers();

		template<class MeshType>
		void UpdateInstancesBuffer();

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		CommandListManager& m_commandListManager;
		std::size_t m_commandListIndex = 0;

		std::deque<Microsoft::WRL::ComPtr<ID3D12Resource>> m_temporaryUploadBuffers;
		FramesResources m_framesResources;

		std::unique_ptr<StandardRenderItem> m_renderRectangle;
		std::deque<std::unique_ptr<StandardRenderItem>> m_renderItems;
		std::unordered_map<RenderEngine::RenderLayer, std::deque<StandardRenderItem*>> m_renderItemsPerLayer;
		std::unordered_map<std::string, StandardRenderItem*> m_renderItemsPerGeometry;

		std::unordered_map<std::wstring, Texture> m_textures;
		DescriptorHeap m_texturesDescriptorHeap;

		std::unordered_map<std::string, std::uint32_t> m_materialIndices;
		std::unordered_map<std::wstring, std::uint32_t> m_textureIndices;
		
		std::unordered_map<std::string, std::deque<StandardRenderItem*>> m_renderItemsPerSkinnedMesh;
		std::unordered_map<StandardRenderItem*, std::uint32_t> m_skinnedRenderItemsMaterialIndices;

		std::size_t m_currentFrameIndex = 0;
	};
}
