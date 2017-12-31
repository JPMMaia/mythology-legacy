#include "pch.h"
#include "RenderRectangle.h"
#include "DirectX12Engine/Geometry/VertexTypes.h"
#include "DirectX12Engine/Geometry/Buffers/VertexBuffer.h"
#include "DirectX12Engine/Geometry/Buffers/IndexBuffer.h"
#include "DirectX12Engine/Geometry/Submesh.h"
#include "DirectX12Engine/Geometry/ImmutableMesh.h"
#include "RenderEngine/Geometry/MeshGenerator.h"

using namespace DirectX12Engine;
using namespace RenderEngine;

StandardRenderItem RenderRectangle::Create(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, Microsoft::WRL::ComPtr<ID3D12Resource>& vertexUploadBuffer, Microsoft::WRL::ComPtr<ID3D12Resource>& indexUploadBuffer)
{
	using VertexType = VertexTypes::PositionTextureCoordinatesVextex;

	// Create mesh data:
	auto meshData = MeshGenerator::CreateRectangle(0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0);
	auto vertices = VertexType::CreateFromMeshData(meshData);

	// Create buffers:
	VertexBuffer vertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexType), vertexUploadBuffer);
	IndexBuffer indexBuffer(d3dDevice, commandList, meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT, indexUploadBuffer);

	// Create mesh:		
	auto mesh = std::make_shared<ImmutableMesh>("RenderRectangle", std::move(vertexBuffer), std::move(indexBuffer));
	mesh->AddSubmesh("Submesh", Submesh(static_cast<std::uint32_t>(meshData.Indices.size())));

	return StandardRenderItem(*d3dDevice, mesh, "Submesh");
}
