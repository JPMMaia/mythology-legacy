#include "StandardMaterial.h"

using namespace GameEngine;

StandardMaterial::StandardMaterial() :
	m_baseColor({ 0.0f, 0.0f, 0.0f, 1.0f })
{
}
StandardMaterial::StandardMaterial(const std::string& name, const Eigen::Vector4f& baseColor, const std::wstring& albedoMapName) :
	m_name(name),
	m_baseColor(baseColor),
	m_albedoMapName(albedoMapName)
{
}

const std::string& StandardMaterial::GetName() const
{
	return m_name;
}
void StandardMaterial::SetName(const std::string& name)
{
	m_name = name;
}

const Eigen::Vector4f& StandardMaterial::GetBaseColor() const
{
	return m_baseColor;
}
void StandardMaterial::SetBaseColor(const Eigen::Vector4f& baseColor)
{
	m_baseColor = baseColor;
}

const std::wstring& StandardMaterial::GetAlbedoMapName() const
{
	return m_albedoMapName;
}
void StandardMaterial::SetAlbedoMapName(const std::wstring& albedoMapName)
{
	m_albedoMapName = albedoMapName;
}
