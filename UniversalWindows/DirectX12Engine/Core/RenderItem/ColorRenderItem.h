#pragma once

#include "GraphicsEngineInterfaces/IRenderItem.h"
#include "Core/Material/ColorMaterial.h"
#include "Core/Geometry/ImmutableMesh.h"

namespace DirectX12Engine
{
	class ColorRenderItem : public GraphicsEngine::IRenderItem
	{
	public:
		ColorRenderItem(const std::string& name, const std::weak_ptr<ImmutableMesh>& mesh, const std::weak_ptr<ColorMaterial>& material);

		const std::string& Name() const;

	private:
		std::string m_name;
		std::weak_ptr<ImmutableMesh> m_mesh;
		std::weak_ptr<ColorMaterial> m_material;
	};
}
