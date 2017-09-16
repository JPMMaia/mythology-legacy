#include "pch.h"
#include "StandardMaterial.h"

#include <set>

using namespace GameEngine;

std::size_t StandardMaterial::GetTextureCount()
{
	std::set<std::wstring> texturePaths;
	std::for_each(StandardMaterial::begin(), StandardMaterial::end(), [&texturePaths](auto& material)
	{
		texturePaths.emplace(material.GetBaseColorTextureName());
	});

	return texturePaths.size();
}

StandardMaterial::StandardMaterial(const std::string & name, const Eigen::Vector4f & baseColorFactor, const std::wstring & baseColorTextureName, float metallicFactor, float roughnessFactor, const std::wstring & metallicRoughnessTextureName) :
	m_name(name),
	m_baseColorFactor(baseColorFactor),
	m_baseColorTextureName(baseColorTextureName),
	m_metallicFactor(metallicFactor),
	m_roughnessFactor(roughnessFactor),
	m_metallicRoughnessTextureName(metallicRoughnessTextureName)
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

const Eigen::Vector4f& StandardMaterial::GetBaseColorFactor() const
{
	return m_baseColorFactor;
}
void StandardMaterial::SetBaseColorFactor(const Eigen::Vector4f& baseColorFactor)
{
	m_baseColorFactor = baseColorFactor;
}

const std::wstring& StandardMaterial::GetBaseColorTextureName() const
{
	return m_baseColorTextureName;
}
void StandardMaterial::SetBaseColorTextureName(const std::wstring& baseColorTextureName)
{
	m_baseColorTextureName = baseColorTextureName;
}

float StandardMaterial::GetMetallicFactor() const
{
	return m_metallicFactor;
}
void StandardMaterial::SetMetallicFactor(float metallicFactor)
{
	m_metallicFactor = metallicFactor;
}

float StandardMaterial::GetRoughnessFactor() const
{
	return m_roughnessFactor;
}
void StandardMaterial::SetRoughnessFactor(float roughnessFactor)
{
	m_roughnessFactor = roughnessFactor;
}

const std::wstring& StandardMaterial::GetMetallicRoughnessTextureName() const
{
	return m_metallicRoughnessTextureName;
}
void StandardMaterial::SetMetallicRoughnessTextureName(const std::wstring& metallicRoughnessTextureName)
{
	m_metallicRoughnessTextureName = metallicRoughnessTextureName;
}
