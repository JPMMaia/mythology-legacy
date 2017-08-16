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

using namespace Common;
using namespace DirectX;
using namespace DirectX12Engine;
using namespace GameEngine;

StandardScene::StandardScene(const std::shared_ptr<DeviceResources>& deviceResources, CommandListManager& commandListManager, const std::shared_ptr<Mythology::MythologyGame>& game) :
	m_deviceResources(deviceResources),
	m_commandListManager(commandListManager),
	m_materialsGPUBuffer(GPUAllocator<ShaderBufferTypes::MaterialData>(deviceResources->GetD3DDevice(), false)),
	m_passGPUBuffer(GPUAllocator<ShaderBufferTypes::PassData>(deviceResources->GetD3DDevice(), false)),
	m_game(game)
{
}
StandardScene::~StandardScene()
{
}

void StandardScene::CreateDeviceDependentResources()
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();

	m_commandListIndex = 0;
	auto commandList = m_commandListManager.GetGraphicsCommandList(m_commandListIndex);

	// Create render rectangle:
	{
		m_temporaryUploadBuffers.emplace_back(); auto& vertexUploadBuffer = m_temporaryUploadBuffers.back();
		m_temporaryUploadBuffers.emplace_back(); auto& indexUploadBuffer = m_temporaryUploadBuffers.back();
		m_renderRectangle = std::make_unique<StandardRenderItem>(RenderRectangle::Create(d3dDevice, commandList, vertexUploadBuffer, indexUploadBuffer));
	}

	// Create render items:
	{
		using VertexType = VertexTypes::PositionNormalTextureCoordinatesVertex;

		CreateRenderItems<MeshComponent<BoxGeometry>, VertexType>(d3dDevice, commandList);
		CreateRenderItems<MeshComponent<RectangleGeometry>, VertexType>(d3dDevice, commandList);
		CreateRenderItems<MeshComponent<CustomGeometry<EigenMeshData>>, VertexType>(d3dDevice, commandList);
	}

	{
		// Reserve space for material instances:
		m_materialsGPUBuffer.reserve(StandardMaterial::GetStorage().size());

		// Create textures descriptor heap:
		{
			// Count number of textures:
			auto textureCount = StandardMaterial::GetTextureCount();

			// Create descriptor heap:
			m_texturesDescriptorHeap.CreateDeviceDependentResources(m_deviceResources, textureCount, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
		}

		std::for_each(StandardMaterial::begin(), StandardMaterial::end(), [this, d3dDevice, commandList](auto& material)
		{
			CreateMaterial(d3dDevice, commandList, material);
		});
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

	m_passGPUBuffer.reserve(1);
	m_passGPUBuffer.push_back(ShaderBufferTypes::PassData());
}
void StandardScene::CreateWindowSizeDependentResources()
{
	auto outputSize = m_deviceResources->GetOutputSize();
	auto aspectRatio = outputSize.x / outputSize.y;
	auto fovAngleY = 60.0f * XM_PI / 180.0f;

	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	auto orientation = m_deviceResources->GetOrientationTransform3D();

	const auto& person = m_game->GetPerson();
	auto camera = person.GetComponent<GameEngine::CameraComponent>("Camera");
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
	UpdatePassBuffer();
	UpdateInstancesBuffers();
}

bool StandardScene::Render(const Common::Timer& timer, RenderLayer renderLayer)
{
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	// Bind pass buffer:
	commandList->SetGraphicsRootConstantBufferView(2, m_passGPUBuffer.get_allocator().GetGPUVirtualAddress(0));

	if (renderLayer == RenderLayer::LightingPass)
	{
		m_renderRectangle->RenderNonInstanced(commandList);
		return true;
	}

	// Set textures:
	std::array<ID3D12DescriptorHeap*, 1> descriptorHeaps = { m_texturesDescriptorHeap.Get() };
	commandList->SetDescriptorHeaps(static_cast<UINT>(descriptorHeaps.size()), descriptorHeaps.data());
	commandList->SetGraphicsRootDescriptorTable(3, m_texturesDescriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart());

	// Bind materials buffer:
	commandList->SetGraphicsRootShaderResourceView(1, m_materialsGPUBuffer.get_allocator().GetGPUVirtualAddress(0));

	std::for_each(m_renderItems.begin(), m_renderItems.end(), [commandList](auto& renderItem)
	{
		renderItem.Render(commandList);
	});

	return true;
}

template <class MeshType, class VertexType>
void StandardScene::CreateRenderItems(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList)
{
	std::for_each(MeshType::begin(), MeshType::end(), [this, d3dDevice, commandList](auto& meshType)
	{
		// Create mesh data:
		auto meshData = meshType.GetGeometry().GenerateMeshData<EigenMeshData>();
		auto vertices = VertexType::CreateFromMeshData(meshData);

		// Create temporary upload buffers:
		m_temporaryUploadBuffers.emplace_back(); auto& vertexUploadBuffer = m_temporaryUploadBuffers.back();
		m_temporaryUploadBuffers.emplace_back(); auto& indexUploadBuffer = m_temporaryUploadBuffers.back();

		// Create buffers:
		VertexBuffer vertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexType), vertexUploadBuffer);
		IndexBuffer indexBuffer(d3dDevice, commandList, meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT, indexUploadBuffer);

		// Create mesh:
		auto mesh = std::make_shared<ImmutableMesh>("", std::move(vertexBuffer), std::move(indexBuffer));
		mesh->AddSubmesh("Submesh", Submesh(meshData));

		StandardRenderItem renderItem(d3dDevice, mesh, "Submesh");

		m_renderItems.emplace_back(std::move(renderItem));
	});
}

void StandardScene::CreateMaterial(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const GameEngine::StandardMaterial& material)
{
	CreateTexture(d3dDevice, commandList, material.GetAlbedoMapName(), true);

	ShaderBufferTypes::MaterialData materialData = {};
	materialData.BaseColor = material.GetBaseColor();
	materialData.AlbedoMapIndex = m_textureIndices.at(material.GetAlbedoMapName());

	m_materialIndices.emplace(material.GetName(), static_cast<std::uint32_t>(m_materialsGPUBuffer.size()));
	m_materialsGPUBuffer.emplace_back(std::move(materialData));
}
void StandardScene::CreateTexture(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, const std::wstring& path, bool isColorData)
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

void StandardScene::UpdatePassBuffer()
{
	ShaderBufferTypes::PassData passData = {};

	const auto& person = m_game->GetPerson();

	// Matrices:
	{
		auto camera = person.GetComponent<GameEngine::CameraComponent>("Camera");

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

	m_passGPUBuffer[0] = passData;
}
void StandardScene::UpdateInstancesBuffers()
{
	auto renderItem = m_renderItems.begin();
	UpdateInstancesBuffer<MeshComponent<BoxGeometry>>(renderItem);
	UpdateInstancesBuffer<MeshComponent<RectangleGeometry>>(renderItem);
	UpdateInstancesBuffer<MeshComponent<CustomGeometry<EigenMeshData>>>(renderItem);
}

template<class MeshType>
void StandardScene::UpdateInstancesBuffer(std::deque<StandardRenderItem>::iterator& renderItem)
{
	std::for_each(MeshType::begin(), MeshType::end(), [this, &renderItem](auto& mesh)
	{
		renderItem->SetInstanceCount(mesh.GetInstanceCount());

		std::size_t index = 0;
		std::for_each(mesh.InstancesBegin(), mesh.InstancesEnd(), [this, &index, &renderItem](auto& instance)
		{
			ShaderBufferTypes::InstanceData shaderData;

			// Update material index:
			shaderData.MaterialIndex = m_materialIndices.at(instance.GetMaterial()->GetName());

			// Update model matrix:
			shaderData.ModelMatrix = instance.GetTransform().GetWorldTransform();

			// Update render item instance:
			renderItem->UpdateInstance(index++, shaderData);
		});

		++renderItem;
	});
}
