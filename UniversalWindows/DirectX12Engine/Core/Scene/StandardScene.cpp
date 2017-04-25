#include "pch.h"
#include "StandardScene.h"
#include "Core/Geometry/ImmutableMesh.h"
#include "Core/Geometry/MeshGenerator.h"
#include "Core/Geometry/VertexTypes.h"
#include "Core/Material/ColorMaterial.h"
#include "Common/Helpers.h"

using namespace Common;
using namespace DirectX12Engine;

StandardScene::StandardScene(const std::shared_ptr<DeviceResources>& deviceResources, CommandListManager& commandListManager) :
	m_deviceResources(deviceResources),
	m_commandListManager(commandListManager)
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

	{
		// Create mesh data:
		auto meshData = MeshGenerator::CreateBox(1.0f, 1.0f, 1.0f, 0);
		auto vertices = VertexTypes::PositionVertex::CreateFromMeshData(meshData);

		// Create buffers:
		VertexBuffer vertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexTypes::PositionVertex));
		IndexBuffer indexBuffer(d3dDevice, commandList, meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT);

		// Create mesh:		
		auto mesh = std::make_shared<ImmutableMesh>("CubeMesh", std::move(vertexBuffer), std::move(indexBuffer));
		mesh->AddSubmesh("CubeSubmesh", Submesh(meshData));
		m_meshes.emplace(mesh->Name(), mesh);

		// Create material:
		auto material = std::make_shared<ColorMaterial>("BlueMaterial");
		material->SetColor({ 0.0f, 0.0f, 1.0f });
		m_materials.emplace(material->Name(), material);

		m_cubeRenderItem = std::make_unique<StandardRenderItem>(mesh, "CubeSubmesh");
	}

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
			m_materialsDataBuffer = std::make_unique<MaterialDataFrameResources>(m_deviceResources, 1, false);
			ShaderBufferTypes::MaterialData materialData;
			materialData.BaseColor = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			auto materialInstance = m_materialsDataBuffer->AddResource(std::move(materialData));

			m_instancesDataBuffer = std::make_unique<InstanceDataFrameResources>(m_deviceResources, 1, false);
			ShaderBufferTypes::InstanceData instanceData;
			XMStoreFloat4x4(&instanceData.ModelMatrix, DirectX::XMMatrixIdentity());
			instanceData.MaterialIndex = 0; // TODO
			auto cubeInstance = m_instancesDataBuffer->AddResource(std::move(instanceData));

			m_cubeRenderItem->AddInstance(cubeInstance, materialInstance);
		}

		m_passDataBuffer = std::make_unique<PassDataFrameResources>(m_deviceResources, 1, true);
		ShaderBufferTypes::PassData passData;
		m_passInstance = m_passDataBuffer->AddResource(std::move(passData));
	}
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

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	auto perspectiveMatrix = DirectX::XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);

	auto orientation = m_deviceResources->GetOrientationTransform3D();
	auto orientationMatrix = XMLoadFloat4x4(&orientation);

	{
		ShaderBufferTypes::PassData passData;

		XMStoreFloat4x4(
			&passData.ProjectionMatrix,
			XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

		// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
		static const DirectX::XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
		static const DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
		static const DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

		XMStoreFloat4x4(&passData.ViewMatrix, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));

		m_passInstance->Update(passData);
	}
}

void StandardScene::SaveState()
{
}
void StandardScene::LoadState()
{
}

void StandardScene::FrameUpdate(const Common::Timer& timer)
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();

	m_materialsDataBuffer->FrameUpdate(d3dDevice);

	m_cubeRenderItem->FrameUpdate();
	m_instancesDataBuffer->FrameUpdate(d3dDevice);
	m_passDataBuffer->FrameUpdate(d3dDevice);
}

bool StandardScene::Render(const Common::Timer& timer)
{
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	// Bind pass buffer:
	commandList->SetGraphicsRootConstantBufferView(2, m_passDataBuffer->GetCurrentFrameBufferGPUVirtualAddress());

	// Bind materials buffer:
	commandList->SetGraphicsRootShaderResourceView(1, m_materialsDataBuffer->GetCurrentFrameBufferGPUVirtualAddress());

	// Bind instances buffer:
	commandList->SetGraphicsRootShaderResourceView(0, m_instancesDataBuffer->GetCurrentFrameBufferGPUVirtualAddress());

	// Render cube:
	m_cubeRenderItem->Render(commandList);
	
	return true;
}

StandardRenderItem* StandardScene::GetCubeRenderItem() const
{
	return m_cubeRenderItem.get();
}
