#pragma once

#include "Interfaces/IRenderItem.h"
#include "DirectX12/Resources/ResourceInstance.h"
#include "DirectX12/Shader/ShaderBufferTypes.h"

namespace GraphicsEngine
{
	class ImmutableMesh;

	class StandardRenderItem : public IRenderItem
	{
	public:
		using InstanceType = ResourceInstance<ShaderBufferTypes::InstanceData>;
		using MaterialType = ResourceInstance<ShaderBufferTypes::MaterialData>;

	public:
		StandardRenderItem(std::shared_ptr<ImmutableMesh> mesh, const std::string& submeshName);

		void AddInstance(const std::shared_ptr<InstanceType>& instance, const std::shared_ptr<MaterialType>& material);

		void FrameUpdate() const;
		void Render(ID3D12GraphicsCommandList* commandList) const;

	private:
		std::string m_submeshName;
		std::shared_ptr<ImmutableMesh> m_mesh;
		
		std::shared_ptr<InstanceType> m_instance;
		std::shared_ptr<MaterialType> m_material;
	};
}
