#pragma once

#include "RenderEngine/IScene.h"
#include "RenderLayer.h"

namespace VulkanEngine
{
	class DeviceManager;
	class Renderer;
	class RenderItem;

	class Scene : public RenderEngine::IScene
	{
	public:
		explicit Scene(std::shared_ptr<Renderer> renderer);

	public:
		void RecreateDeviceDependentResources() override;
		void RecreateWindowSizeDependentResources() override;

	public:
		void FrameUpdate(const Common::Timer& timer) override;
		bool Render(const RenderEngine::RenderParameters& renderParameters) override;

	public:
		void CreateMesh(const std::string& name, const RenderEngine::MeshData<RenderEngine::Vertex, std::uint16_t>& meshData) override;
		void CreateMesh(const std::string& name, const RenderEngine::MeshData<RenderEngine::Vertex, std::uint32_t>& meshData) override;
		void DeleteMesh(const std::string& name) override;

		void CreateMaterial(const std::string& name, const RenderEngine::Material& material) override;
		void UpdateMaterial(const std::string& name, const RenderEngine::Material& material) override;
		void DeleteMaterial(const std::string& name) override;

		void CreateInstance(const std::string& meshName, const std::shared_ptr<RenderEngine::Instance>& instance) override;
		void UpdateInstance(const std::string& meshName, const RenderEngine::Instance& instance) override;
		void DeleteInstance(const std::string& meshName, const RenderEngine::Instance& instance) override;

	private:
		template <class IndexType>
		void CreateMesh(const std::string& name, const RenderEngine::MeshData<RenderEngine::Vertex, IndexType>& meshData);

	private:
		void AddRenderItem(std::unique_ptr<RenderItem> renderItem, std::initializer_list<RenderLayer> renderLayers);
		void RemoveRenderItem();

	private:
		std::shared_ptr<Renderer> m_renderer;
		std::vector<std::unique_ptr<RenderItem>> m_renderItems;
		std::unordered_map<RenderLayer, std::vector<RenderItem*>> m_renderItemsPerLayer;
	};
}
