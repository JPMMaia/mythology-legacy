#include "pch.h"
#include "RenderRectangle.h"
#include "Core/Geometry/VertexTypes.h"
#include "Core/Geometry/Buffers/VertexBuffer.h"
#include "Core/Geometry/Buffers/IndexBuffer.h"
#include "Core/Geometry/Submesh.h"
#include "Core/Geometry/ImmutableMesh.h"

using namespace DirectX12Engine;
using namespace GameEngine;

StandardRenderItem RenderRectangle::Create(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList)
{
	using VertexType = VertexTypes::PositionTextureCoordinatesVextex;

	// Create mesh data:
	auto meshData = EigenMeshGenerator::CreateRectangle(0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 0);
	auto vertices = VertexType::CreateFromMeshData(meshData);

	// Create buffers:
	VertexBuffer vertexBuffer(d3dDevice, commandList, vertices.data(), vertices.size(), sizeof(VertexType));
	IndexBuffer indexBuffer(d3dDevice, commandList, meshData.Indices.data(), meshData.Indices.size(), sizeof(uint32_t), DXGI_FORMAT_R32_UINT);

	// Create mesh:		
	auto mesh = std::make_shared<ImmutableMesh>("RenderRectangle", std::move(vertexBuffer), std::move(indexBuffer));
	mesh->AddSubmesh("Submesh", Submesh(meshData));

	return StandardRenderItem(d3dDevice, mesh, "Submesh");
}
