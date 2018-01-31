#include "pch.h"
#include "Scene.h"
#include "VulkanEngine/Renderer.h"
#include "VulkanEngine/Buffers/GeometryBuffer.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"
#include "VulkanEngine/RenderItems/RenderItem.h"
#include "VulkanEngine/Devices/DeviceManager.h"
#include "VulkanEngine/Scenes/RenderParameters.h"
#include "VulkanEngine/Geometry/Vertex.h"

using namespace VulkanEngine;

Scene::Scene(std::shared_ptr<Renderer> renderer) :
	m_renderer(std::move(renderer))
{
}

void Scene::RecreateDeviceDependentResources()
{	
	DestroyRenderItems();
	CreateRenderItems();
}
void Scene::RecreateWindowSizeDependentResources()
{
}

void Scene::FrameUpdate(const Common::Timer& timer)
{
}
bool Scene::Render(const RenderEngine::RenderParameters& renderParameters)
{
	const auto& device = m_renderer->GetDeviceManager().GetDevice();
	
	for (const auto& renderItem : m_renderItemsPerLayer[renderParameters.RenderLayer])
		renderItem->Draw(device, renderParameters.CommandBuffer);

	return true;
}

template <class VertexType0, class VertexType1, class IndexType>
void Scene::CreateMesh(MeshContainer<VertexType0, IndexType>& meshes, const std::string& name, const RenderEngine::MeshData<VertexType1, IndexType>& meshData)
{
	auto vertices = typename VertexType0::CreateFromVertexData(meshData.Vertices);
	const auto& indices = meshData.Indices;
	meshes.emplace(name, RenderEngine::MeshData<VertexType0, IndexType>(vertices, indices));
}
void Scene::CreateMesh(const std::string& name, const RenderEngine::MeshData<RenderEngine::Vertex, std::uint16_t>& meshData)
{
	CreateMesh(m_smallMeshes, name, meshData);
}
void Scene::CreateMesh(const std::string& name, const RenderEngine::MeshData<RenderEngine::Vertex, std::uint32_t>& meshData)
{
	CreateMesh(m_largeMeshes, name, meshData);
}
void Scene::DeleteMesh(const std::string& name)
{
	m_smallMeshes.erase(name);
	m_largeMeshes.erase(name);
}

void Scene::CreateMaterial(const std::string& name, const RenderEngine::Material& material)
{
}
void Scene::UpdateMaterial(const std::string& name, const RenderEngine::Material& material)
{
}
void Scene::DeleteMaterial(const std::string& name)
{
}

void Scene::CreateInstance(const std::string& meshName, const std::shared_ptr<RenderEngine::Instance>& instance)
{
}
void Scene::UpdateInstance(const std::string& meshName, const RenderEngine::Instance& instance)
{
}
void Scene::DeleteInstance(const std::string& meshName, const RenderEngine::Instance& instance)
{
}

void Scene::CreateRenderItems()
{
	auto createRenderItems = [this](const auto& meshes)
	{
		for (const auto& nameMeshPair : meshes)
		{
			const auto& mesh = nameMeshPair.second;

			// Calculate vertex and index data sizes:
			auto vertexBufferSize = mesh.GetVertexDataByteSize();
			auto indexBufferSize = mesh.GetIndexDataByteSize();

			// Create the geometry buffer and schedule an upload of buffer data:
			GeometryBuffer geometryBuffer(m_renderer->GetDeviceManager(), vertexBufferSize, indexBufferSize, ToVulkanIndexType<std::remove_reference_t<decltype(mesh)>::IndexType>());
			m_renderer->UploadBufferData(geometryBuffer, vertexBufferSize, indexBufferSize, mesh);

			// Specify submesh parameters:
			SubmeshGeometry submesh(
				static_cast<std::uint32_t>(mesh.Indices.size()),
				1,
				0,
				0,
				0
			);

			// Create render item:
			AddRenderItem(std::make_unique<RenderItem>(std::move(geometryBuffer), std::move(submesh)), { RenderLayer::Opaque });
		}
	};

	createRenderItems(m_smallMeshes);
	createRenderItems(m_largeMeshes);
}
void Scene::DestroyRenderItems()
{
	m_renderItemsPerLayer.clear();
	m_renderItems.clear();
}

void Scene::AddRenderItem(std::unique_ptr<RenderItem> renderItem, std::initializer_list<RenderLayer> renderLayers)
{
	for (auto renderLayer : renderLayers)
		m_renderItemsPerLayer[renderLayer].emplace_back(renderItem.get());	
		
	m_renderItems.emplace_back(std::move(renderItem));
}
