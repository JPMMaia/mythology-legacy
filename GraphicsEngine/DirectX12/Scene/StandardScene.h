#pragma once

#include "Interfaces/IScene.h"
#include "Interfaces/IRenderItem.h"
#include "Interfaces/IMaterial.h"
#include "DirectX12/DeviceResources.h"
#include "DirectX12/Command/CommandListManager.h"
#include "DirectX12/Geometry/IMesh.h"

#include <unordered_map>
#include "DirectX12/RenderItem/StandardRenderItem.h"

namespace GraphicsEngine
{
	class StandardScene : public IScene
	{
	public:
		StandardScene(const std::shared_ptr<DeviceResources>& deviceResources, CommandListManager& commandListManager);
		~StandardScene();

	public:
		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;
		
		void SaveState() override;
		void LoadState() override;

		void FrameUpdate(const Common::Timer& timer) override;
		void FixedUpdate(const Common::Timer& timer) override;

		StandardRenderItem* GetCubeRenderItem() const;

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		CommandListManager& m_commandListManager;
		size_t m_commandListIndex = 0;

		std::unordered_map<std::string, std::shared_ptr<IMesh>> m_meshes;
		std::unordered_map<std::string, std::shared_ptr<IMaterial>> m_materials;
		std::unordered_map<std::string, std::shared_ptr<IRenderItem>> m_renderItems;

		std::unique_ptr<StandardRenderItem> m_cubeRenderItem;
		std::shared_ptr<ResourceInstance<ShaderBufferTypes::PassData>> m_passInstance;

		using InstanceDataFrameResources = FrameResource<ResourceInstance<ShaderBufferTypes::InstanceData>, ShaderBufferTypes::InstanceData>;
		std::unique_ptr<InstanceDataFrameResources> m_instancesDataBuffer;

		using MaterialDataFrameResources = FrameResource<ResourceInstance<ShaderBufferTypes::MaterialData>, ShaderBufferTypes::MaterialData>;
		std::unique_ptr<MaterialDataFrameResources> m_materialsDataBuffer;;

		using PassDataFrameResources = FrameResource<ResourceInstance<ShaderBufferTypes::PassData>, ShaderBufferTypes::PassData>;
		std::unique_ptr<PassDataFrameResources> m_passDataBuffer;
	};
}
