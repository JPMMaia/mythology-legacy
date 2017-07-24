#pragma once

#include "GraphicsEngineInterfaces/IRenderItem.h"
#include "Core/Resources/ResourceInstance.h"
#include "Core/Shader/ShaderBufferTypes.h"

namespace DirectX12Engine
{
	class ImmutableMesh;

	class StandardRenderItem : public GraphicsEngine::IRenderItem
	{
	public:
		using InstanceType = ResourceInstance<ShaderBufferTypes::InstanceData>;
		using MaterialType = ResourceInstance<ShaderBufferTypes::MaterialData>;

	public:
		StandardRenderItem() = default;
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
