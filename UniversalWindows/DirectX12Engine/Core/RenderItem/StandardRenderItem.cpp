#include "pch.h"
#include "StandardRenderItem.h"
#include "Core/Geometry/ImmutableMesh.h"

using namespace DirectX12Engine;

StandardRenderItem::StandardRenderItem(std::shared_ptr<ImmutableMesh> mesh, const std::string& submeshName) :
	m_submeshName(submeshName),
	m_mesh(mesh)
{
}

void StandardRenderItem::AddInstance(const std::shared_ptr<InstanceType>& instance, const std::shared_ptr<MaterialType>& material)
{
	m_instance = instance;
	m_material = material;
}

void StandardRenderItem::FrameUpdate() const
{
	ShaderBufferTypes::InstanceData data;
	XMStoreFloat4x4(&data.ModelMatrix, DirectX::XMMatrixIdentity());
	data.MaterialIndex = static_cast<uint32_t>(m_material->GetShaderIndex());
	m_instance->Update(data);
}

void StandardRenderItem::Render(ID3D12GraphicsCommandList* commandList) const
{
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_mesh->Render(commandList, m_submeshName);
}
