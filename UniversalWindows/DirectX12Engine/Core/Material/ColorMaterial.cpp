#include "pch.h"
#include "ColorMaterial.h"

using namespace DirectX12Engine;

ColorMaterial::ColorMaterial(const std::string& name) :
	m_name(name)
{
}

const std::string& ColorMaterial::Name() const
{
	return m_name;
}

DirectX::XMFLOAT3 ColorMaterial::Color() const
{
	return m_color;
}
void ColorMaterial::SetColor(const DirectX::XMFLOAT3& color)
{
	m_color = color;
}
