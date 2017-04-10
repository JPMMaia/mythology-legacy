#pragma once

#include "Interfaces/IRenderItem.h"
#include "DirectX12/Material/ColorMaterial.h"
#include "DirectX12/Geometry/ImmutableMesh.h"

namespace GraphicsEngine
{
	class ColorRenderItem : public IRenderItem
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
