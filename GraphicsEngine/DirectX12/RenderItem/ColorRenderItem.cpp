#include "pch.h"
#include "ColorRenderItem.h"

using namespace GraphicsEngine;

ColorRenderItem::ColorRenderItem(const std::string& name, const std::weak_ptr<ImmutableMesh>& mesh, const std::weak_ptr<ColorMaterial>& material) :
m_name(name),
m_mesh(mesh),
m_material(material)
{
}

const std::string& ColorRenderItem::Name() const
{
	return m_name;
}
