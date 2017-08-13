#pragma once

#include "GraphicsEngineInterfaces/IScene.h"
#include "Core/DeviceResources.h"
#include "Core/Command/CommandListManager.h"
#include "Core/RenderItem/StandardRenderItem.h"
#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Core/Textures/Texture.h"
#include "Mythology/MythologyGame.h"

#include <queue>
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
		template<class MeshType, class VertexType>
		void CreateRenderItems(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList);

		void CreateTexture(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::wstring& path, bool isColorData);

		void UpdatePassBuffer();
		void UpdateInstancesBuffers();

		template<class MeshType>
		void UpdateInstancesBuffer(std::deque<StandardRenderItem>::iterator& renderItem);

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		CommandListManager& m_commandListManager;
		std::size_t m_commandListIndex = 0;

		std::queue<Microsoft::WRL::ComPtr<ID3D12Resource>> m_temporaryUploadBuffers;
		GPUUploadBuffer<ShaderBufferTypes::MaterialData> m_materialsGPUBuffer;
		GPUUploadBuffer<ShaderBufferTypes::PassData> m_passGPUBuffer;

		std::unique_ptr<StandardRenderItem> m_renderRectangle;
		std::deque<StandardRenderItem> m_renderItems;
		std::unordered_map<std::wstring, Texture> m_textures;

		std::unordered_map<std::string, std::uint32_t> m_materialIndices;

		std::shared_ptr<Mythology::MythologyGame> m_game;
	};
}
