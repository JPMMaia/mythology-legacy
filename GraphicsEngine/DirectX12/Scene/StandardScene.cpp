#include "pch.h"
#include "StandardScene.h"
#include "DirectX12/Geometry/ImmutableMesh.h"
#include "DirectX12/Geometry/MeshGenerator.h"
#include "DirectX12/Geometry/VertexTypes.h"
#include "DirectX12/Material/ColorMaterial.h"
#include "DirectX12/RenderItem/ColorRenderItem.h"

using namespace GraphicsEngine;

StandardScene::StandardScene(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
}
StandardScene::~StandardScene()
{
}

void StandardScene::CreateDeviceDependentResources()
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();

	{
		// Create mesh data:
		auto meshData = MeshGenerator::CreateBox(1.0f, 1.0f, 1.0f, 0);
		auto vertices = VertexTypes::PositionVertex::CreateFromMeshData(meshData);
		
		// Create buffers:
		VertexBuffer vertexBuffer(d3dDevice, m_commandList.Get(), vertices.data(), vertices.size(), sizeof(VertexTypes::PositionVertex));
		IndexBuffer indexBuffer(d3dDevice, m_commandList.Get(), meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT);

		// Create mesh:		
		auto mesh = std::make_shared<ImmutableMesh>("CubeMesh", std::move(vertexBuffer), std::move(indexBuffer));
		mesh->AddSubmesh("CubeSubmesh", Submesh(meshData));
		m_meshes.emplace(mesh->Name(), mesh);

		// Create material:
		auto material = std::make_shared<ColorMaterial>("BlueMaterial");
		material->SetColor({ 0.0f, 0.0f, 1.0f });
		m_materials.emplace(material->Name(), material);

		// Create render item:
		auto renderItem = std::make_shared<ColorRenderItem>("CubeRenderItem", mesh, material);
		m_renderItems.emplace(renderItem->Name(), renderItem);
	}
}
void StandardScene::CreateWindowSizeDependentResources()
{
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
void StandardScene::FixedUpdate(const Common::Timer& timer)
{
}
