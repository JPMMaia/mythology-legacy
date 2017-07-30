#include "pch.h"
#include "StandardScene.h"
#include "Core/Geometry/ImmutableMesh.h"
#include "Core/Geometry/MeshGenerator.h"
#include "Core/Geometry/VertexTypes.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Common/Helpers.h"
#include "RenderLayers.h"


using namespace Common;
using namespace DirectX12Engine;

StandardScene::StandardScene(const std::shared_ptr<DeviceResources>& deviceResources, CommandListManager& commandListManager) :
	m_deviceResources(deviceResources),
	m_commandListManager(commandListManager),
	m_materialsGPUBuffer(GPUAllocator<ShaderBufferTypes::MaterialData>(deviceResources->GetD3DDevice(), false)),
	m_passGPUBuffer(GPUAllocator<ShaderBufferTypes::PassData>(deviceResources->GetD3DDevice(), false)),
	m_cubeRenderItem(deviceResources->GetD3DDevice()),
	m_rectangleRenderItem(deviceResources->GetD3DDevice())
{
}
StandardScene::~StandardScene()
{
}

void StandardScene::CreateDeviceDependentResources()
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();

	ID3D12GraphicsCommandList* commandList;
	m_commandListIndex = m_commandListManager.CreateGraphicsCommandList(commandList);

	// Cube Render Item:
	{
		using VertexType = VertexTypes::PositionNormalVertex;

		// Create mesh data:
		//auto meshData = MeshGenerator::CreateBox(1.0f, 1.0f, 1.0f, 0);
		auto meshData = MeshGenerator::CreateRectangle(-10.0f, 0.0f, 20.0f, 20.0f, 0.0f);
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
			// Make a material:
			ShaderBufferTypes::MaterialData materialData;
			materialData.BaseColor = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			m_materialsGPUBuffer.push_back(materialData);

			// Make an instance:
			ShaderBufferTypes::InstanceData instanceData;
			instanceData.MaterialIndex = 0;
			auto rotation = DirectX::XMMatrixRotationX(-90.0f * DirectX::XM_PI / 180.0f);
			XMStoreFloat4x4(&instanceData.ModelMatrix, rotation);
			m_cubeRenderItem.AddInstance(instanceData);
		}
	}

	m_passGPUBuffer.reserve(1);
	m_passGPUBuffer.push_back(ShaderBufferTypes::PassData());
}
void StandardScene::CreateWindowSizeDependentResources()
{
	auto outputSize = m_deviceResources->GetOutputSize();
	auto aspectRatio = outputSize.x / outputSize.y;
	auto fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	auto orientation = m_deviceResources->GetOrientationTransform3D();
	auto orientationMatrix = XMLoadFloat4x4(&orientation);

	m_camera = Camera(aspectRatio, fovAngleY, 0.25f, 50.0f, orientationMatrix);
	m_camera.SetPosition(0.0f, 2.0f, -5.0f);
	m_camera.Update();

	UpdatePassBuffer();
}

void StandardScene::SaveState()
{
}
void StandardScene::LoadState()
{
}

void StandardScene::ProcessInput()
{
	static constexpr auto movementSensibility = 0.125f;
	auto& keyboard = m_deviceResources->Keyboard();
	if (keyboard.IsKeyDown('W'))
		m_camera.MoveForward(movementSensibility);
	if (keyboard.IsKeyDown('S'))
		m_camera.MoveForward(-movementSensibility);
	if (keyboard.IsKeyDown('D'))
		m_camera.MoveRight(movementSensibility);
	if (keyboard.IsKeyDown('A'))
		m_camera.MoveRight(-movementSensibility);

	static constexpr auto tiltSensibility = 0.0625f;
	if (keyboard.IsKeyDown('Q'))
		m_camera.RotateWorldZ(-tiltSensibility);
	if (keyboard.IsKeyDown('E'))
		m_camera.RotateWorldZ(tiltSensibility);

	static constexpr auto mouseSensibility = 1.0f / 512.0f;
	auto& mouse = m_deviceResources->Mouse();
	auto deltaMovement = mouse.DeltaMovement();
	m_camera.RotateWorldX(-mouseSensibility * deltaMovement[1]);
	m_camera.RotateWorldY(-mouseSensibility * deltaMovement[0]);
	
	m_camera.Update();
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
	
	return true;
}

StandardRenderItem& StandardScene::GetCubeRenderItem()
{
	return m_cubeRenderItem;
}

void StandardScene::UpdatePassBuffer()
{
	ShaderBufferTypes::PassData passData;

	const auto& viewMatrix = m_camera.GetViewMatrix();
	const auto& projectionMatrix = m_camera.GetProjectionMatrix();
	auto viewProjectionMatrix = viewMatrix * projectionMatrix;
	auto viewProjectionMatrixDeterminant = XMMatrixDeterminant(viewProjectionMatrix);
	auto inverseViewProjectionMatrix = XMMatrixInverse(&viewProjectionMatrixDeterminant, viewProjectionMatrix);

	XMStoreFloat4x4(&passData.ViewMatrix, XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(&passData.ProjectionMatrix, XMMatrixTranspose(projectionMatrix));
	XMStoreFloat4x4(&passData.ViewProjectionMatrix, XMMatrixTranspose(viewProjectionMatrix));
	XMStoreFloat4x4(&passData.InverseViewProjectionMatrix, XMMatrixTranspose(inverseViewProjectionMatrix));
	XMStoreFloat3(&passData.CameraPositionW, m_camera.GetPosition());

	m_passGPUBuffer[0] = passData;
}
