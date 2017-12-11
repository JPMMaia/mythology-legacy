#include "pch.h"
#include "StandardScene.h"
#include "Core/Geometry/ImmutableMesh.h"
#include "Core/Geometry/VertexTypes.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "RenderLayers.h"
#include "Core/Textures/Texture.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Geometry/Primitives/BoxGeometry.h"
#include "GameEngine/Geometry/Primitives/RectangleGeometry.h"
#include "GameEngine/Geometry/EigenGeometry.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "Core/RenderItem/Specific/RenderRectangle.h"
#include "GameEngine/Geometry/Primitives/CustomGeometry.h"
#include "GameEngine/Component/Meshes/SkinnedMeshComponent.h"
#include "Common/Timer.h"

using namespace Common;
using namespace DirectX;
using namespace DirectX12Engine;
using namespace GameEngine;

template<>
void StandardScene::CreateRenderItems<SkinnedMeshComponent>(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList);

template<>
void StandardScene::UpdateInstancesBuffer<SkinnedMeshComponent>();

StandardScene::StandardScene(const std::shared_ptr<DeviceResources>& deviceResources, CommandListManager& commandListManager, const std::shared_ptr<Mythology::MythologyGame>& game) :
	m_deviceResources(deviceResources),
	m_commandListManager(commandListManager),
	m_framesResources(deviceResources),
	m_game(game)
{
	MaterialEventsQueue::OnCreate += { "StandardScene", this, &StandardScene::OnMaterialCreated };
	MaterialEventsQueue::OnDelete += { "StandardScene", this, &StandardScene::OnMaterialDeleted };

	MeshEventsQueue::OnCreate += { "StandardScene", this, &StandardScene::OnMeshesCreated };
	MeshEventsQueue::OnDelete += { "StandardScene", this, &StandardScene::OnMeshesDeleted };

	InstanceEventsQueue::OnCreate += { "StandardScene", this, &StandardScene::OnInstancesCreated };
	InstanceEventsQueue::OnUpdate += { "StandardScene", this, &StandardScene::OnInstancesUpdated };
	InstanceEventsQueue::OnDelete += { "StandardScene", this, &StandardScene::OnInstancesDeleted };
}
StandardScene::~StandardScene()
{
}

void StandardScene::CreateDeviceDependentResources()
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();

	m_commandListIndex = 0;
	auto commandList = m_commandListManager.GetGraphicsCommandList(m_commandListIndex);

	// Create textures descriptor heap:
	{
		// Count number of textures:
		auto textureCount = StandardMaterial::GetTextureCount();

		// Create descriptor heap:
		m_texturesDescriptorHeap.CreateDeviceDependentResources(m_deviceResources, textureCount, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	}

	// Create environment textures:
	{
		CreateEmptyTexture(d3dDevice, commandList, L"Environmetal0", true);
	}

	MaterialEventsQueue::Flush();

	// Create render rectangle:
	{
		m_temporaryUploadBuffers.emplace_back(); auto& vertexUploadBuffer = m_temporaryUploadBuffers.back();
		m_temporaryUploadBuffers.emplace_back(); auto& indexUploadBuffer = m_temporaryUploadBuffers.back();
		m_renderRectangle = std::make_unique<StandardRenderItem>(RenderRectangle::Create(d3dDevice, commandList, vertexUploadBuffer, indexUploadBuffer));
	}

	// Create render items:
	{
		MeshEventsQueue::Flush();
		CreateRenderItems<SkinnedMeshComponent>(d3dDevice, commandList);
	}

	// Upload vertex and index buffers to the GPU in order to dispose the upload buffers:
	{
		commandList->Close();
		m_commandListManager.ExecuteCommandList(m_commandListIndex);

		// Wait for the command list to finish executing; the content of the upload buffers need to be uploaded to the GPU before disposing the upload buffers:
		m_deviceResources->WaitForGpu();

		// Dispose upload buffers:
		m_temporaryUploadBuffers.clear();
	}

	{
		auto& passBuffer = m_framesResources.PassBuffer;
		passBuffer.Reserve(1);
		passBuffer.Emplace();
	}
}
void StandardScene::CreateWindowSizeDependentResources()
{
	auto outputSize = m_deviceResources->GetOutputSize();
	auto aspectRatio = outputSize.x / outputSize.y;
	auto fovAngleY = XM_PI / 2.0f;

	auto orientation = m_deviceResources->GetOrientationTransform3D();

	auto camera = m_game->GetMainCamera();
	camera->SetAspectRatio(aspectRatio);
	camera->SetFovAngleY(fovAngleY);
	camera->SetOrientationMatrix(Eigen::Affine3f(orientation));
}

void StandardScene::SaveState()
{
}
void StandardScene::LoadState()
{
}

void StandardScene::ProcessInput()
{
}
void StandardScene::FrameUpdate(const Common::Timer& timer)
{
	MaterialEventsQueue::Flush();
	MeshEventsQueue::Flush();

	UpdatePassBuffer();
	UpdateSkinnedAnimationBuffers();
	UpdateInstancesBuffers();

	m_framesResources.Update(m_deviceResources->GetCurrentFrameIndex());
}

bool StandardScene::Render(const Common::Timer& timer, RenderLayer renderLayer)
{
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);
	std::size_t frameIndex = m_deviceResources->GetCurrentFrameIndex();

	// Bind pass buffer:
	{
		auto& passBuffer = m_framesResources.PassBuffer;
		commandList->SetGraphicsRootConstantBufferView(2, passBuffer.GetGPUVirtualAddress(frameIndex, 0));
	}

	if (renderLayer == RenderLayer::LightingPass)
	{
		m_renderRectangle->RenderNonInstanced(*commandList);
		return true;
	}

	// Set textures:
	std::array<ID3D12DescriptorHeap*, 1> descriptorHeaps = { m_texturesDescriptorHeap.Get() };
	commandList->SetDescriptorHeaps(static_cast<UINT>(descriptorHeaps.size()), descriptorHeaps.data());
	commandList->SetGraphicsRootDescriptorTable(3, m_texturesDescriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart());

	// Bind materials buffer:
	{
		auto& materialsBuffer = m_framesResources.MaterialsBuffer;
		commandList->SetGraphicsRootShaderResourceView(1, materialsBuffer.GetGPUVirtualAddress(frameIndex, 0));
	}

	if (renderLayer == RenderLayer::Opaque)
	{
		const auto& renderItems = m_renderItemsPerLayer.at(RenderLayer::Opaque);
		for (auto renderItemIt = renderItems.begin(); renderItemIt != renderItems.end(); ++renderItemIt)
		{
			(*renderItemIt)->RenderInstanced(*commandList, m_framesResources, frameIndex);
		}
	}
	else if (renderLayer == RenderLayer::SkinnedOpaque)
	{
		auto& skinnedMeshAnimationBuffer = m_framesResources.SkinnedMeshAnimationBuffer;
		auto& skinnedMeshInstancesBuffer = m_framesResources.SkinnedMeshInstancesBuffer;

		std::size_t bufferIndex = 0;
		std::size_t skinnedMeshInstanceStartIndex = 0;
		for (auto skinnedMeshComponentIt = SkinnedMeshComponent::begin(); skinnedMeshComponentIt != SkinnedMeshComponent::end(); ++skinnedMeshComponentIt)
		{
			auto& mesh = *skinnedMeshComponentIt;

			const auto& renderItems = m_renderItemsPerSkinnedMesh.at(mesh.GetName());
			for (std::size_t i = 0; i < mesh.GetInstancesCount(); ++i)
			{
				// Bind skinned constant buffer:
				commandList->SetGraphicsRootConstantBufferView(4, skinnedMeshAnimationBuffer.GetGPUVirtualAddress(frameIndex, bufferIndex++));

				// Render:
				auto materialBufferIndex = skinnedMeshInstanceStartIndex;
				for (auto renderItemIt = renderItems.begin(); renderItemIt != renderItems.end(); ++renderItemIt)
				{
					// Bind materials' buffer:
					commandList->SetGraphicsRootConstantBufferView(5, skinnedMeshInstancesBuffer.GetGPUVirtualAddress(frameIndex, materialBufferIndex++));

					(*renderItemIt)->RenderNonInstanced(*commandList);
				}
			}
			skinnedMeshInstanceStartIndex += renderItems.size();
		}
	}

	return true;
}

VertexBuffer StandardScene::CreateVertexBuffer(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const EigenMeshData& meshData, bool isSkinned)
{
	// Create a temporary upload buffer:
	m_temporaryUploadBuffers.emplace_back();
	auto& vertexUploadBuffer = m_temporaryUploadBuffers.back();

	if (isSkinned)
	{
		using VertexType = VertexTypes::PositionNormalTextureCoordinatesSkinnedVertex;
		auto vertices = VertexType::CreateFromMeshData(meshData);
		return VertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexType), vertexUploadBuffer);
	}
	else
	{
		using VertexType = VertexTypes::PositionNormalTextureCoordinatesVertex;
		auto vertices = VertexType::CreateFromMeshData(meshData);
		return VertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexType), vertexUploadBuffer);
	}
}
IndexBuffer StandardScene::CreateIndexBuffer(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const EigenMeshData& meshData)
{
	// Create a temporary upload buffer:
	m_temporaryUploadBuffers.emplace_back();
	auto& indexUploadBuffer = m_temporaryUploadBuffers.back();

	return IndexBuffer(d3dDevice, commandList, meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT, indexUploadBuffer);
}

template <class MeshType>
void StandardScene::CreateRenderItems(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList)
{
	for (auto meshIt = MeshType::begin(); meshIt != MeshType::end(); ++meshIt)
	{
		auto& meshType = *meshIt;

		// Create mesh data:
		auto meshData = meshType.GetGeometry().GenerateMeshData<EigenMeshData>();

		// Create render item:
		CreateRenderItem(d3dDevice, commandList, meshType.GetName(), meshData);
	}
}
template<>
void StandardScene::CreateRenderItems<SkinnedMeshComponent>(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList)
{
	for (auto meshIt = SkinnedMeshComponent::begin(); meshIt != SkinnedMeshComponent::end(); ++meshIt)
	{
		auto& meshType = *meshIt;

		const auto& geometries = meshType.GetGeometries();
		const auto& materials = meshType.GetMaterials();

		std::deque<StandardRenderItem*> skinnedRenderItems;
		for (std::size_t geometryIndex = 0; geometryIndex < geometries.size(); ++geometryIndex)
		{
			const auto& geometry = geometries[geometryIndex];
			const auto& material = materials[geometryIndex];

			// Create mesh data:
			auto meshData = geometry.GenerateMeshData<EigenMeshData>();

			// Create buffers:
			auto vertexBuffer = CreateVertexBuffer(d3dDevice, commandList, meshData, true);
			auto indexBuffer = CreateIndexBuffer(d3dDevice, commandList, meshData);

			// Create mesh:
			auto mesh = std::make_shared<ImmutableMesh>(std::to_string(m_renderItems.size()), std::move(vertexBuffer), std::move(indexBuffer));
			mesh->AddSubmesh("Submesh", Submesh(meshData));

			// Create render item:
			auto renderItem = std::make_unique<StandardRenderItem>(*d3dDevice, mesh, "Submesh");
			skinnedRenderItems.push_back(renderItem.get());
			m_skinnedRenderItemsMaterialIndices.emplace(renderItem.get(), m_materialIndices.at(material->GetName()));
			m_renderItems.emplace_back(std::move(renderItem));
		}

		m_renderItemsPerSkinnedMesh.emplace(meshType.GetName(), std::move(skinnedRenderItems));
	}
}

void StandardScene::CreateRenderItem(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::string& meshName, const EigenMeshData& meshData)
{
	// Create buffers:
	auto vertexBuffer = CreateVertexBuffer(d3dDevice, commandList, meshData, false);
	auto indexBuffer = CreateIndexBuffer(d3dDevice, commandList, meshData);

	// Create mesh:
	auto mesh = std::make_shared<ImmutableMesh>(std::to_string(m_renderItems.size()), std::move(vertexBuffer), std::move(indexBuffer));
	mesh->AddSubmesh("Submesh", Submesh(meshData));

	// Create render item:
	auto renderItem = std::make_unique<StandardRenderItem>(*d3dDevice, mesh, "Submesh");
	auto layer = RenderLayer::Opaque;
	m_renderItemsPerLayer[layer].emplace_back(renderItem.get());
	m_renderItemsPerGeometry.emplace(meshName, renderItem.get());
	m_renderItems.emplace_back(std::move(renderItem));
}
void StandardScene::CreateMaterial(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const GameEngine::StandardMaterial& material)
{
	CreateTextureFromFile(d3dDevice, commandList, material.GetBaseColorTextureName(), true);
	CreateTextureFromFile(d3dDevice, commandList, material.GetMetallicRoughnessTextureName(), false);

	ShaderBufferTypes::MaterialData materialData = {};
	materialData.BaseColorFactor = material.GetBaseColorFactor();
	materialData.BaseColorTextureIndex = m_textureIndices.at(material.GetBaseColorTextureName());
	materialData.MetallicFactor = material.GetMetallicFactor();
	materialData.RoughnessFactor = material.GetRoughnessFactor();
	materialData.MetallicRoughnessTextureIndex = m_textureIndices.at(material.GetMetallicRoughnessTextureName());

	auto& materialsBuffer = m_framesResources.MaterialsBuffer;
	auto index = materialsBuffer.Emplace(std::move(materialData));
	m_materialIndices.emplace(material.GetName(), static_cast<std::uint32_t>(index));
}
void StandardScene::CreateTextureFromFile(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::wstring& path, bool isColorData)
{
	// Check if texture was already created:
	if (m_textures.find(path) != m_textures.end())
		return;

	// Create temporary upload buffer:
	m_temporaryUploadBuffers.emplace_back();
	auto& uploadBuffer = m_temporaryUploadBuffers.back();

	// Create texture:
	Texture texture;

	// Load texture:
	DDS_ALPHA_MODE alphaMode;
	bool isCubeMap;
	texture.LoadTextureFromFile(
		d3dDevice,
		commandList,
		path,
		D3D12_RESOURCE_FLAG_NONE,
		isColorData ? DDS_LOADER_FORCE_SRGB : DDS_LOADER_DEFAULT,
		alphaMode,
		isCubeMap,
		uploadBuffer
	);

	// Create shader resource view:
	texture.CreateShaderResourceView(m_deviceResources, m_texturesDescriptorHeap, "SRV");

	// Add index of texture in the descriptor heap:
	m_textureIndices.emplace(path, static_cast<uint32_t>(m_textures.size()));

	// Add texture:
	m_textures.emplace(path, std::move(texture));
}
void StandardScene::CreateEmptyTexture(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList * commandList, const std::wstring & name, bool isColorData)
{
	// TODO
}

void StandardScene::UpdatePassBuffer()
{
	ShaderBufferTypes::PassData passData = {};

	// Matrices:
	{
		auto camera = m_game->GetMainCamera();

		const auto& viewMatrix = camera->GetViewMatrix();
		const auto& projectionMatrix = camera->GetProjectionMatrix();
		auto viewProjectionMatrix = projectionMatrix * viewMatrix;
		auto inverseViewProjectionMatrix = viewProjectionMatrix.inverse();

		passData.ViewMatrix = viewMatrix.matrix();
		passData.ProjectionMatrix = projectionMatrix.matrix();
		passData.ViewProjectionMatrix = viewProjectionMatrix.matrix();
		passData.InverseViewProjectionMatrix = inverseViewProjectionMatrix.matrix();
		passData.CameraPositionW = camera->GetTransform().GetWorldPosition();
	}

	// Lights:
	{
		auto pointLightIt = PointLightComponent::begin();
		auto end = PointLightComponent::end();

		for (std::size_t i = 0; i < ShaderBufferTypes::PassData::MaxNumLights && pointLightIt != end; ++i)
		{
			auto& light = passData.Lights[i];
			light.Strength = pointLightIt->GetStrength();
			light.FalloffStart = pointLightIt->GetFalloffStart();
			light.FalloffEnd = pointLightIt->GetFalloffEnd();
			light.Position = pointLightIt->GetWorldPosition();

			++pointLightIt;
		}
	}

	{
		auto& passBuffer = m_framesResources.PassBuffer;
		passBuffer.Set(0, passData);
	}
}
void StandardScene::UpdateSkinnedAnimationBuffers()
{
	std::size_t animationCount = 0;
	std::for_each(SkinnedMeshComponent::begin(), SkinnedMeshComponent::end(), [&animationCount](const SkinnedMeshComponent& mesh)
	{
		animationCount += mesh.GetInstancesCount();
	});

	auto& skinnedMeshAnimationBuffer = m_framesResources.SkinnedMeshAnimationBuffer;
	skinnedMeshAnimationBuffer.Resize(animationCount);

	std::size_t bufferIndex = 0;
	for (auto meshIt = SkinnedMeshComponent::begin(); meshIt != SkinnedMeshComponent::end(); ++meshIt)
	{
		auto& mesh = *meshIt;
		for (auto instanceIt = mesh.GetInstancesBegin(); instanceIt != mesh.GetInstancesEnd(); ++instanceIt)
		{
			auto& instance = *instanceIt;

			ShaderBufferTypes::SkinnedAnimationData skinnedData;

			// Update bone transforms:
			{
				const auto& finalTransforms = instance.GetAnimation().GetFinalTransforms();
				const auto minSize = (std::min)(finalTransforms.size(), ShaderBufferTypes::SkinnedAnimationData::MaxNumBones);
				std::copy_n(finalTransforms.begin(), minSize, skinnedData.BoneTransforms.begin());
			}

			// Update model matrix:
			skinnedData.ModelMatrix = instance.GetTransform().GetWorldTransform();

			skinnedMeshAnimationBuffer.Set(bufferIndex++, std::move(skinnedData));
		}
	}
}
void StandardScene::UpdateInstancesBuffers()
{
	InstanceEventsQueue::Flush();
	//UpdateInstancesBuffer<MeshComponent<BoxGeometry>>();
	//UpdateInstancesBuffer<MeshComponent<RectangleGeometry>>();
	//UpdateInstancesBuffer<MeshComponent<CustomGeometry<EigenMeshData>>>();
	UpdateInstancesBuffer<SkinnedMeshComponent>();
}

template<class MeshType>
void StandardScene::UpdateInstancesBuffer()
{
	for (auto meshIt = MeshType::begin(); meshIt != MeshType::end(); ++meshIt)
	{
		auto renderItem = m_renderItemsPerGeometry.at(meshIt->GetName());
		renderItem->SetInstanceCount(m_framesResources, meshIt->GetInstanceCount());

		std::size_t index = 0;
		for (auto instanceIt = meshIt->InstancesBegin(); instanceIt != meshIt->InstancesEnd(); ++instanceIt)
		{
			ShaderBufferTypes::InstanceData shaderData;

			// Update material index:
			shaderData.MaterialIndex = m_materialIndices.at(instanceIt->GetMaterial()->GetName());

			// Update model matrix:
			shaderData.ModelMatrix = instanceIt->GetTransform().GetWorldTransform();

			// Update render item instance:
			renderItem->UpdateInstance(m_framesResources, index++, shaderData);
		}

		++renderItem;
	}
}
template<>
void StandardScene::UpdateInstancesBuffer<SkinnedMeshComponent>()
{
	std::size_t instancesCount = 0;
	std::for_each(SkinnedMeshComponent::begin(), SkinnedMeshComponent::end(), [this, &instancesCount](SkinnedMeshComponent& mesh)
	{
		const auto& renderItems = m_renderItemsPerSkinnedMesh.at(mesh.GetName());

		instancesCount += renderItems.size();
	});

	auto& skinnedMeshInstancesBuffer = m_framesResources.SkinnedMeshInstancesBuffer;
	skinnedMeshInstancesBuffer.Resize(instancesCount);

	auto materialBufferIndex = 0;
	for (auto skinnedMeshComponentIt = SkinnedMeshComponent::begin(); skinnedMeshComponentIt != SkinnedMeshComponent::end(); ++skinnedMeshComponentIt)
	{
		auto& mesh = *skinnedMeshComponentIt;
		const auto& materials = mesh.GetMaterials();

		for (std::size_t i = 0; i < materials.size(); ++i)
		{
			ShaderBufferTypes::SkinnedMeshData shaderData = {};

			// Update material index:
			shaderData.MaterialIndex = m_materialIndices.at(materials[i]->GetName());

			// Update skinned mesh buffer:
			skinnedMeshInstancesBuffer.Set(materialBufferIndex++, shaderData);
		}
	}
}

void StandardScene::OnMaterialCreated(MaterialEventsQueue::EventArg materials)
{
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	auto& materialsBuffer = m_framesResources.MaterialsBuffer;
	materialsBuffer.Reserve(materialsBuffer.GetSize() + materials.size());

	for(const auto& material : materials)
		CreateMaterial(m_deviceResources->GetD3DDevice(), commandList, *material);
}
void StandardScene::OnMaterialDeleted(MaterialEventsQueue::EventArg materials)
{
	// TODO
}

void StandardScene::OnMeshesCreated(MeshEventsQueue::EventArg meshes)
{
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	for(const auto& mesh : meshes)
		CreateRenderItem(m_deviceResources->GetD3DDevice(), commandList, mesh->GetName(), mesh->GenerateMeshData());
}
void StandardScene::OnMeshesDeleted(MeshEventsQueue::EventArg meshes)
{
	// TODO
}

void StandardScene::OnInstancesCreated(InstanceEventsQueue::EventArg meshInstances)
{
	for (const auto& instances : meshInstances) 
	{
		auto renderItem = m_renderItemsPerGeometry.at(instances.first);
		
		auto newInstanceCount = instances.second.size();
		auto oldCount = renderItem->GetInstanceCount(m_framesResources);
		auto newCount = oldCount + newInstanceCount;
		renderItem->ReserveSpaceForInstances(m_framesResources, newCount);
		
		for (const auto& instance : instances.second)
		{
			ShaderBufferTypes::InstanceData shaderData;
			shaderData.MaterialIndex = m_materialIndices.at(instance->GetMaterial()->GetName());
			shaderData.ModelMatrix = instance->GetTransform().GetWorldTransform();

			renderItem->AddInstance(m_framesResources, instance->GetRenderInfo(), shaderData);
		}
	}
}
void StandardScene::OnInstancesUpdated(InstanceEventsQueue::EventArg meshInstances)
{
	for (const auto& instances : meshInstances)
	{
		auto renderItem = m_renderItemsPerGeometry.at(instances.first);

		for (const auto& instance : instances.second)
		{
			ShaderBufferTypes::InstanceData shaderData;
			shaderData.MaterialIndex = m_materialIndices.at(instance->GetMaterial()->GetName());
			shaderData.ModelMatrix = instance->GetTransform().GetWorldTransform();

			renderItem->UpdateInstance(m_framesResources, *instance->GetRenderInfo(), shaderData);
		}
	}
}
void StandardScene::OnInstancesDeleted(InstanceEventsQueue::EventArg meshInstances)
{
	for (const auto& instances : meshInstances)
	{
		auto renderItem = m_renderItemsPerGeometry.at(instances.first);

		for (const auto& instance : instances.second)
		{
			renderItem->DeleteInstance(m_framesResources, *instance->GetRenderInfo());
		}
	}
}
