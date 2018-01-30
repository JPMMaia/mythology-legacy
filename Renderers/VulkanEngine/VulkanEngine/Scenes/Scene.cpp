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

template <class IndexType>
void Scene::CreateMesh(const std::string& name, const RenderEngine::MeshData<RenderEngine::Vertex, IndexType>& meshData)
{
	RenderEngine::MeshData<Vertex, IndexType> specificMeshData;
	specificMeshData.Vertices = Vertex::CreateFromVertexData(meshData.Vertices);
	specificMeshData.Indices = meshData.Indices;

	auto vertexBufferSize = specificMeshData.GetVertexDataByteSize();
	auto indexBufferSize = specificMeshData.GetIndexDataByteSize();
	GeometryBuffer geometryBuffer(m_renderer->GetDeviceManager(), vertexBufferSize, indexBufferSize, ToVulkanIndexType<IndexType>());

	// TODO move this to set function of renderer
	m_renderer->UploadBufferData(geometryBuffer, vertexBufferSize, indexBufferSize, specificMeshData);

	SubmeshGeometry submesh(
		static_cast<std::uint32_t>(specificMeshData.Indices.size()),
		1,
		0,
		0,
		0
	);

	AddRenderItem(std::make_unique<RenderItem>(std::move(geometryBuffer), std::move(submesh)), { RenderLayer::Opaque });
}
void Scene::CreateMesh(const std::string& name, const RenderEngine::MeshData<RenderEngine::Vertex, std::uint16_t>& meshData)
{
	CreateMesh<std::uint16_t>(name, meshData);
}
void Scene::CreateMesh(const std::string& name, const RenderEngine::MeshData<RenderEngine::Vertex, std::uint32_t>& meshData)
{
	CreateMesh<std::uint32_t>(name, meshData);
}
void Scene::DeleteMesh(const std::string& name)
{
	RemoveRenderItem();
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

void Scene::AddRenderItem(std::unique_ptr<RenderItem> renderItem, std::initializer_list<RenderLayer> renderLayers)
{
	for (auto renderLayer : renderLayers)
		m_renderItemsPerLayer[renderLayer].emplace_back(renderItem.get());	
		
	m_renderItems.emplace_back(std::move(renderItem));
}
void Scene::RemoveRenderItem()
{
	// TODO
}
