#include "pch.h"
#include "StandardMaterial.h"

#include <set>

using namespace GameEngine;

StandardMaterial::StandardMaterial(const std::string& name, const RenderEngine::Material& data) :
	m_name(name),
	m_data(data)
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

const RenderEngine::Material& StandardMaterial::GetData() const
{
	return m_data;
}

const Eigen::Vector4f& StandardMaterial::GetBaseColorFactor() const
{
	return m_data.BaseColorFactor;
}
void StandardMaterial::SetBaseColorFactor(const Eigen::Vector4f& baseColorFactor)
{
	m_data.BaseColorFactor = baseColorFactor;
}

const std::wstring& StandardMaterial::GetBaseColorTextureName() const
{
	return m_data.BaseColorTextureName;
}
void StandardMaterial::SetBaseColorTextureName(const std::wstring& baseColorTextureName)
{
	m_data.BaseColorTextureName = baseColorTextureName;
}

float StandardMaterial::GetMetallicFactor() const
{
	return m_data.MetallicFactor;
}
void StandardMaterial::SetMetallicFactor(float metallicFactor)
{
	m_data.MetallicFactor = metallicFactor;
}

float StandardMaterial::GetRoughnessFactor() const
{
	return m_data.RoughnessFactor;
}
void StandardMaterial::SetRoughnessFactor(float roughnessFactor)
{
	m_data.RoughnessFactor = roughnessFactor;
}

const std::wstring& StandardMaterial::GetMetallicRoughnessTextureName() const
{
	return m_data.MetallicRoughnessTextureName;
}
void StandardMaterial::SetMetallicRoughnessTextureName(const std::wstring& metallicRoughnessTextureName)
{
	m_data.MetallicRoughnessTextureName = metallicRoughnessTextureName;
}
