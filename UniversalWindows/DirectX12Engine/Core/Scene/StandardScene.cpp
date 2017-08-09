#include "pch.h"
#include "StandardScene.h"
#include "Core/Geometry/ImmutableMesh.h"
#include "Core/Geometry/MeshGenerator.h"
#include "Core/Geometry/VertexTypes.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "RenderLayers.h"
#include "Core/Textures/Texture.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"

using namespace Common;
using namespace DirectX;
using namespace DirectX12Engine;

StandardScene::StandardScene(const std::shared_ptr<DeviceResources>& deviceResources, CommandListManager& commandListManager, const std::shared_ptr<Mythology::MythologyGame>& game) :
	m_deviceResources(deviceResources),
	m_commandListManager(commandListManager),
	m_materialsGPUBuffer(GPUAllocator<ShaderBufferTypes::MaterialData>(deviceResources->GetD3DDevice(), false)),
	m_passGPUBuffer(GPUAllocator<ShaderBufferTypes::PassData>(deviceResources->GetD3DDevice(), false)),
	m_cubeRenderItem(deviceResources->GetD3DDevice()),
	m_rectangleRenderItem(deviceResources->GetD3DDevice()),
	m_xAxis(deviceResources->GetD3DDevice()),
	m_yAxis(deviceResources->GetD3DDevice()),
	m_zAxis(deviceResources->GetD3DDevice()),
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

	// Cube Render Item:
	{
		using VertexType = VertexTypes::PositionNormalTextureCoordinatesVertex;

		// Create mesh data:
		auto meshData = MeshGenerator::CreateBox(1.0f, 1.0f, 1.0f, 0);
		//auto meshData = MeshGenerator::CreateRectangle(-10.0f, 0.0f, 20.0f, 20.0f, 0.0f);
		auto vertices = VertexType::CreateFromMeshData(meshData);

		// Create buffers:
		VertexBuffer vertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexType));
		IndexBuffer indexBuffer(d3dDevice, commandList, meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT);

		// Create mesh:
		auto mesh = std::make_shared<ImmutableMesh>("CubeMesh", std::move(vertexBuffer), std::move(indexBuffer));
		mesh->AddSubmesh("CubeSubmesh", Submesh(meshData));
		m_meshes.emplace(mesh->Name(), mesh);

		m_cubeRenderItem = StandardRenderItem(d3dDevice, mesh, "CubeSubmesh");
	}

	// X-axis:
	{
		using VertexType = VertexTypes::PositionNormalTextureCoordinatesVertex;

		// Create mesh data:
		auto meshData = MeshGenerator::CreateBox(2.0f, 0.1f, 0.1f, 0);
		auto vertices = VertexType::CreateFromMeshData(meshData);

		// Create buffers:
		VertexBuffer vertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexType));
		IndexBuffer indexBuffer(d3dDevice, commandList, meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT);

		// Create mesh:
		auto mesh = std::make_shared<ImmutableMesh>("X-axis", std::move(vertexBuffer), std::move(indexBuffer));
		mesh->AddSubmesh("Submesh", Submesh(meshData));
		m_meshes.emplace(mesh->Name(), mesh);

		m_xAxis = StandardRenderItem(d3dDevice, mesh, "Submesh");
	}

	// Y-axis:
	{
		using VertexType = VertexTypes::PositionNormalTextureCoordinatesVertex;

		// Create mesh data:
		auto meshData = MeshGenerator::CreateBox(0.1f, 2.0f, 0.1f, 0);
		auto vertices = VertexType::CreateFromMeshData(meshData);

		// Create buffers:
		VertexBuffer vertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexType));
		IndexBuffer indexBuffer(d3dDevice, commandList, meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT);

		// Create mesh:
		auto mesh = std::make_shared<ImmutableMesh>("Y-axis", std::move(vertexBuffer), std::move(indexBuffer));
		mesh->AddSubmesh("Submesh", Submesh(meshData));
		m_meshes.emplace(mesh->Name(), mesh);

		m_yAxis = StandardRenderItem(d3dDevice, mesh, "Submesh");
	}

	// Z-axis:
	{
		using VertexType = VertexTypes::PositionNormalTextureCoordinatesVertex;

		// Create mesh data:
		auto meshData = MeshGenerator::CreateBox(0.1f, 0.1f, 2.0f, 0);
		auto vertices = VertexType::CreateFromMeshData(meshData);

		// Create buffers:
		VertexBuffer vertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexType));
		IndexBuffer indexBuffer(d3dDevice, commandList, meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT);

		// Create mesh:
		auto mesh = std::make_shared<ImmutableMesh>("Z-axis", std::move(vertexBuffer), std::move(indexBuffer));
		mesh->AddSubmesh("Submesh", Submesh(meshData));
		m_meshes.emplace(mesh->Name(), mesh);

		m_zAxis = StandardRenderItem(d3dDevice, mesh, "Submesh");
	}

	// Render Rectangle Render Item
	{
		using VertexType = VertexTypes::PositionTextureCoordinatesVextex;

		// Create mesh data:
		auto meshData = MeshGenerator::CreateRectangle(-1.0f, 1.0f, 2.0f, 2.0f, 0.0f);
		auto vertices = VertexType::CreateFromMeshData(meshData);

		// Create buffers:
		VertexBuffer vertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexType));
		IndexBuffer indexBuffer(d3dDevice, commandList, meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT);

		// Create mesh:		
		auto mesh = std::make_shared<ImmutableMesh>("RectangleMesh", std::move(vertexBuffer), std::move(indexBuffer));
		mesh->AddSubmesh("RectangleSubmesh", Submesh(meshData));
		m_meshes.emplace(mesh->Name(), mesh);

		m_rectangleRenderItem = StandardRenderItem(d3dDevice, mesh, "RectangleSubmesh");
	}

	// Upload vertex and index buffers to the GPU in order to dispose the upload buffers:
	{
		commandList->Close();
		m_commandListManager.ExecuteCommandList(m_commandListIndex);

		// Wait for the command list to finish executing; the vertex/index buffers need to be uploaded to the GPU before disposing the upload buffers:
		m_deviceResources->WaitForGpu();

		for (const auto& mesh : m_meshes)
			mesh.second->DisposeUploadBuffers();
	}

	{
		{
			// Make materials:
			{
				m_materialsGPUBuffer.reserve(4);

				ShaderBufferTypes::MaterialData materialData;
				materialData.BaseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
				materialData.AlbedoMapIndex = 0;
				m_materialsGPUBuffer.emplace_back(materialData);

				materialData.BaseColor = { 1.0f, 0.0f, 0.0f, 1.0f };
				m_materialsGPUBuffer.emplace_back(materialData);

				materialData.BaseColor = { 0.0f, 1.0f, 0.0f, 1.0f };
				m_materialsGPUBuffer.emplace_back(materialData);

				materialData.BaseColor = { 0.0f, 0.0f, 1.0f, 1.0f };
				m_materialsGPUBuffer.emplace_back(materialData);
			}

			// Make an instance:
			ShaderBufferTypes::InstanceData instanceData;
			instanceData.MaterialIndex = 0;
			instanceData.ModelMatrix = Eigen::Affine3f::Identity();
			//instanceData.ModelMatrix = Eigen::AngleAxisf(-90.0f * std::acos(-1.0f) / 180.0f, Eigen::Vector3f::UnitX()).matrix();
			//auto rotation = DirectX::XMMatrixRotationX(-90.0f * DirectX::XM_PI / 180.0f);
			//XMStoreFloat4x4(&instanceData.ModelMatrix, rotation);
			m_cubeRenderItem.AddInstance(instanceData);

			instanceData.MaterialIndex = 1;
			instanceData.ModelMatrix = Eigen::Translation<float, 3>(1.0f, 0.0f, 0.0f);
			m_xAxis.AddInstance(instanceData);

			instanceData.MaterialIndex = 2;
			instanceData.ModelMatrix = Eigen::Translation<float, 3>(0.0f, 1.0f, 0.0f);
			m_yAxis.AddInstance(instanceData);

			instanceData.MaterialIndex = 3;
			instanceData.ModelMatrix = Eigen::Translation<float, 3>(0.0f, 0.0f, 1.0f);
			m_zAxis.AddInstance(instanceData);
		}
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
}

bool StandardScene::Render(const Common::Timer& timer, RenderLayer renderLayer)
{
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	if(renderLayer == RenderLayer::LightingPass)
	{
		m_rectangleRenderItem.RenderNonInstanced(commandList);
		return true;
	}

	// Bind pass buffer:
	commandList->SetGraphicsRootConstantBufferView(2, m_passGPUBuffer.get_allocator().GetGPUVirtualAddress(0));

	// Bind materials buffer:
	commandList->SetGraphicsRootShaderResourceView(1, m_materialsGPUBuffer.get_allocator().GetGPUVirtualAddress(0));

	// Render cube:
	m_cubeRenderItem.Render(commandList);

	m_xAxis.Render(commandList);
	m_yAxis.Render(commandList);
	m_zAxis.Render(commandList);
	
	return true;
}

StandardRenderItem& StandardScene::GetCubeRenderItem()
{
	return m_cubeRenderItem;
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
		/*auto pointLightIt = GameEngine::PointLightComponent::Allocator::begin();

		auto& light = passData.Lights[0];
		light.Strength = pointLightIt->GetStrength();
		light.FalloffStart = pointLightIt->GetFalloffStart();
		light.FalloffEnd = pointLightIt->GetFalloffEnd();
		light.Position = pointLightIt->GetWorldPosition();*/
	}

	m_passGPUBuffer[0] = passData;
}
