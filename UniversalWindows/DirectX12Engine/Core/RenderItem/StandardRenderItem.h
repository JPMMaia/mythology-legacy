#pragma once

#include "GraphicsEngineInterfaces/IRenderItem.h"

namespace DirectX12Engine
{
	class ImmutableMesh;

	class StandardRenderItem : public GraphicsEngine::IRenderItem
	{
	public:
		StandardRenderItem() = default;
		StandardRenderItem(std::shared_ptr<ImmutableMesh> mesh, const std::string& submeshName);

		void Render(ID3D12GraphicsCommandList* commandList) const;

	private:
		std::string m_submeshName;
		std::shared_ptr<ImmutableMesh> m_mesh;
	};
}
