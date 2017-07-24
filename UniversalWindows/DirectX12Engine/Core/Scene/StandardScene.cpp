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
	m_commandListManager(commandListManager),
	m_instancesGPUBuffer(GPUAllocator<ShaderBufferTypes::InstanceData>(m_deviceResources->GetD3DDevice(), false)),
	m_materialsGPUBuffer(GPUAllocator<ShaderBufferTypes::MaterialData>(m_deviceResources->GetD3DDevice(), false)),
	m_passGPUBuffer(GPUAllocator<ShaderBufferTypes::PassData>(m_deviceResources->GetD3DDevice(), false))
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

		m_cubeRenderItem = StandardRenderItem(mesh, "CubeSubmesh");
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
			// Make a material:
			ShaderBufferTypes::MaterialData materialData;
			materialData.BaseColor = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			m_materialsGPUBuffer.push_back(materialData);

			// Make an instance:
			ShaderBufferTypes::InstanceData instanceData;
			instanceData.MaterialIndex = 0;
			XMStoreFloat4x4(&instanceData.ModelMatrix, DirectX::XMMatrixIdentity());
			m_instancesGPUBuffer.push_back(instanceData);
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

		m_passGPUBuffer[0] = passData;
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
}

bool StandardScene::Render(const Common::Timer& timer)
{
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	// Bind pass buffer:
	commandList->SetGraphicsRootConstantBufferView(2, m_passGPUBuffer.get_allocator().GetGPUVirtualAddress(0));

	// Bind materials buffer:
	commandList->SetGraphicsRootShaderResourceView(1, m_materialsGPUBuffer.get_allocator().GetGPUVirtualAddress(0));

	// Bind instances buffer:
	commandList->SetGraphicsRootShaderResourceView(0, m_instancesGPUBuffer.get_allocator().GetGPUVirtualAddress(0));

	// Render cube:
	m_cubeRenderItem.Render(commandList);
	
	return true;
}

StandardRenderItem& StandardScene::GetCubeRenderItem()
{
	return m_cubeRenderItem;
}
