﻿#pragma once

#include "GraphicsEngineInterfaces/IScene.h"
#include "GraphicsEngineInterfaces/IRenderItem.h"
#include "GraphicsEngineInterfaces/IMaterial.h"
#include "Core/DeviceResources.h"
#include "Core/Command/CommandListManager.h"
#include "Core/Geometry/IMesh.h"
#include "Core/RenderItem/StandardRenderItem.h"

#include <unordered_map>
#include "Core/Resources/GPUAllocator.h"

namespace DirectX12Engine
{
	class StandardScene : public GraphicsEngine::IScene
	{
	public:
		StandardScene(const std::shared_ptr<DeviceResources>& deviceResources, CommandListManager& commandListManager);
		~StandardScene();

		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;
		
		void SaveState() override;
		void LoadState() override;

		void FrameUpdate(const Common::Timer& timer) override;

		bool Render(const Common::Timer& timer) override;

		StandardRenderItem& GetCubeRenderItem();

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		CommandListManager& m_commandListManager;
		std::size_t m_commandListIndex = 0;

		std::unordered_map<std::string, std::shared_ptr<IMesh>> m_meshes;
		std::unordered_map<std::string, std::shared_ptr<GraphicsEngine::IMaterial>> m_materials;
		std::unordered_map<std::string, std::shared_ptr<GraphicsEngine::IRenderItem>> m_renderItems;

		StandardRenderItem m_cubeRenderItem;

		

		template<class DataType>
		using GPUUploadBuffer = std::vector<DataType, GPUAllocator<DataType>>;

		GPUUploadBuffer<ShaderBufferTypes::InstanceData> m_instancesGPUBuffer;
		GPUUploadBuffer<ShaderBufferTypes::MaterialData> m_materialsGPUBuffer;
		GPUUploadBuffer<ShaderBufferTypes::PassData> m_passGPUBuffer;

		/*
		std::unique_ptr<StandardRenderItem> m_cubeRenderItem;
		std::shared_ptr<ResourceInstance<ShaderBufferTypes::PassData>> m_passInstance;

		using InstanceDataFrameResources = FrameResource<ResourceInstance<ShaderBufferTypes::InstanceData>, ShaderBufferTypes::InstanceData>;
		std::unique_ptr<InstanceDataFrameResources> m_instancesDataBuffer;

		using MaterialDataFrameResources = FrameResource<ResourceInstance<ShaderBufferTypes::MaterialData>, ShaderBufferTypes::MaterialData>;
		std::unique_ptr<MaterialDataFrameResources> m_materialsDataBuffer;;

		using PassDataFrameResources = FrameResource<ResourceInstance<ShaderBufferTypes::PassData>, ShaderBufferTypes::PassData>;
		std::unique_ptr<PassDataFrameResources> m_passDataBuffer;
		*/
	};
}
