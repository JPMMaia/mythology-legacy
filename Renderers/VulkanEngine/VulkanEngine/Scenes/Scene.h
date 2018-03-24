#pragma once

#include "RenderEngine/IScene.h"
#include "RenderLayer.h"
#include "VulkanEngine/Geometry/Vertex.h"
#include "VulkanEngine/Buffers/UniformBuffer.h"
#include "VulkanEngine/Images/TextureImage.h"

namespace VulkanEngine
{
	class DeviceManager;
	class Renderer;
	class RenderItem;

	class Scene : public RenderEngine::IScene
	{
	public:
		template <class VertexType, class IndexType>
		using MeshContainer = std::unordered_map<std::string, RenderEngine::MeshData<VertexType, IndexType>>;

		using MaterialContainer = std::unordered_map<std::string, RenderEngine::Material>;

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
		template <class VertexType0, class VertexType1, class IndexType>
		static void CreateMesh(MeshContainer<VertexType0, IndexType>& meshes, const std::string& name, const RenderEngine::MeshData<VertexType1, IndexType>& meshData);

	private:
		void CreateRenderItems();
		void DestroyRenderItems();
		void AddRenderItem(std::unique_ptr<RenderItem> renderItem, std::initializer_list<RenderLayer> renderLayers);	

		void CreateBuffers();
		void DestroyBuffers();

		void CreateTextureImages();
		void DestroyTextureImages();

	private:
		std::weak_ptr<Renderer> m_renderer;
		MeshContainer<Vertex, std::uint16_t> m_smallMeshes;
		MeshContainer<Vertex, std::uint32_t> m_largeMeshes;
		MaterialContainer m_materials;
		std::vector<std::unique_ptr<RenderItem>> m_renderItems;
		std::unordered_map<RenderLayer, std::vector<RenderItem*>> m_renderItemsPerLayer;
		std::vector<UniformBuffer> m_passDataBuffers;
		std::vector<UniformBuffer> m_instanceDataBuffers;
		std::vector<TextureImage> m_textureImages;
	};
}
