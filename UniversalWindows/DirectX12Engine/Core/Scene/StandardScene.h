#pragma once

#include "GraphicsEngineInterfaces/IScene.h"
#include "GraphicsEngineInterfaces/IRenderItem.h"
#include "GraphicsEngineInterfaces/IMaterial.h"
#include "Core/DeviceResources.h"
#include "Core/Command/CommandListManager.h"
#include "Core/Geometry/IMesh.h"
#include "Core/RenderItem/StandardRenderItem.h"
#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Mythology/MythologyGame.h"

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

		StandardRenderItem& GetCubeRenderItem();

	private:
		void UpdatePassBuffer();

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		CommandListManager& m_commandListManager;
		std::size_t m_commandListIndex = 0;

		std::unordered_map<std::string, std::shared_ptr<IMesh>> m_meshes;
		std::unordered_map<std::string, std::shared_ptr<GraphicsEngine::IMaterial>> m_materials;
		std::unordered_map<std::string, std::shared_ptr<GraphicsEngine::IRenderItem>> m_renderItems;

		GPUUploadBuffer<ShaderBufferTypes::MaterialData> m_materialsGPUBuffer;
		GPUUploadBuffer<ShaderBufferTypes::PassData> m_passGPUBuffer;

		StandardRenderItem m_cubeRenderItem;
		StandardRenderItem m_floor;
		StandardRenderItem m_rectangleRenderItem;
		
		StandardRenderItem m_xAxis;
		StandardRenderItem m_yAxis;
		StandardRenderItem m_zAxis;

		std::shared_ptr<Mythology::MythologyGame> m_game;
	};
}
