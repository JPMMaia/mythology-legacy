#include "pch.h"
#include "Scene.h"
#include "VulkanEngine/Renderer.h"
#include "VulkanEngine/Buffers/GeometryBuffer.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"
#include "VulkanEngine/RenderItems/RenderItem.h"
#include "VulkanEngine/Devices/DeviceManager.h"
#include "VulkanEngine/Scenes/RenderParameters.h"
#include "VulkanEngine/Geometry/Vertex.h"
#include "VulkanEngine/ShaderData/PassData.h"
#include "VulkanEngine/ShaderData/InstanceData.h"

using namespace VulkanEngine;

Scene::Scene(std::shared_ptr<Renderer> renderer) :
	m_renderer(std::move(renderer))
{
}

void Scene::RecreateDeviceDependentResources()
{
	DestroyRenderItems();
	CreateRenderItems();

	auto renderer = m_renderer.lock();

	{
		m_passDataBuffers.emplace_back(renderer->GetDeviceManager(), sizeof(PassData));
		{
			auto& buffer = m_passDataBuffers.back();

			PassData data{ Eigen::Affine3f::Identity(), Eigen::Affine3f::Identity() };
			buffer.SetData(renderer->GetDeviceManager().GetDevice(), &data, 0, buffer.GetSize());
		}

		m_instanceDataBuffers.emplace_back(renderer->GetDeviceManager(), sizeof(InstanceData));
		{
			auto& buffer = m_instanceDataBuffers.back();

			InstanceData data{ Eigen::Affine3f::Identity() };
			buffer.SetData(renderer->GetDeviceManager().GetDevice(), &data, 0, buffer.GetSize());
		}
	}

	{
		std::array<vk::DescriptorBufferInfo, 2> bufferInfos =
		{
			vk::DescriptorBufferInfo(*m_instanceDataBuffers.back(), 0, sizeof(InstanceData)),
			vk::DescriptorBufferInfo(*m_passDataBuffers.back(), 0, sizeof(PassData)),
		};

		const auto& descriptorSets = renderer->GetDescriptorManager().GetDescriptorSets();
		std::array<vk::WriteDescriptorSet, 2> descriptorWrites =
		{
			vk::WriteDescriptorSet(
				descriptorSets.at(0),
				0, 0,
				1, vk::DescriptorType::eUniformBuffer,
				nullptr, &bufferInfos[0], nullptr
			),
			vk::WriteDescriptorSet(
				descriptorSets.at(1),
				0, 0,
				1, vk::DescriptorType::eUniformBuffer,
				nullptr, &bufferInfos[1], nullptr
			),
		};

		auto device = renderer->GetDeviceManager().GetDevice();
		device.updateDescriptorSets(
			static_cast<std::uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 
			0, nullptr
		);
	}
}
void Scene::RecreateWindowSizeDependentResources()
{
}

void Scene::FrameUpdate(const Common::Timer& timer)
{
	auto device = m_renderer.lock()->GetDeviceManager().GetDevice();
}
bool Scene::Render(const RenderEngine::RenderParameters& renderParameters)
{
	const auto& device = m_renderer.lock()->GetDeviceManager().GetDevice();

	// Draw render items:
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
	auto renderer = m_renderer.lock();

	auto createRenderItems = [this, &renderer](const auto& meshes)
	{
		for (const auto& nameMeshPair : meshes)
		{
			const auto& mesh = nameMeshPair.second;

			// Calculate vertex and index data sizes:
			auto vertexBufferSize = mesh.GetVertexDataByteSize();
			auto indexBufferSize = mesh.GetIndexDataByteSize();

			// Create the geometry buffer and schedule an upload of buffer data:
			GeometryBuffer geometryBuffer(renderer->GetDeviceManager(), vertexBufferSize, indexBufferSize, ToVulkanIndexType<std::remove_reference_t<decltype(mesh)>::IndexType>());
			renderer->UploadBufferData(geometryBuffer, vertexBufferSize, indexBufferSize, mesh);

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
