#include "pch.h"
#include "StandardRenderItem.h"
#include "Core/Geometry/ImmutableMesh.h"

using namespace DirectX12Engine;

StandardRenderItem::StandardRenderItem(std::shared_ptr<ImmutableMesh> mesh, const std::string& submeshName) :
	m_submeshName(submeshName),
	m_mesh(mesh)
{
}

void StandardRenderItem::Render(ID3D12GraphicsCommandList* commandList) const
{
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_mesh->Render(commandList, m_submeshName);
}
