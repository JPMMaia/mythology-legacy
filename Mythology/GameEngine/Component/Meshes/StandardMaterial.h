#pragma once

#include "GameEngine/Memory/UseStorage.h"

namespace GameEngine
{
	class StandardMaterial : public UseStorage<StandardMaterial>
	{
	public:
		static std::size_t GetTextureCount();

	public:
		StandardMaterial() = default;
		StandardMaterial(const std::string& name, const Eigen::Vector4f& baseColorFactor, const std::wstring& baseColorTextureName, float metallicFactor, float roughnessFactor, const std::wstring& metallicRoughnessTextureName);

		const std::string& GetName() const;
		void SetName(const std::string& name);

		const Eigen::Vector4f& GetBaseColorFactor() const;
		void SetBaseColorFactor(const Eigen::Vector4f& baseColorFactor);

		const std::wstring& GetBaseColorTextureName() const;
		void SetBaseColorTextureName(const std::wstring& baseColorTextureName);

		float GetMetallicFactor() const;
		void SetMetallicFactor(float metallicFactor);

		float GetRoughnessFactor() const;
		void SetRoughnessFactor(float roughnessFactor);

		const std::wstring& GetMetallicRoughnessTextureName() const;
		void SetMetallicRoughnessTextureName(const std::wstring& metallicRoughnessTextureName);

	private:
		std::string m_name;
		Eigen::Vector4f m_baseColorFactor;
		std::wstring m_baseColorTextureName;
		float m_metallicFactor;
		float m_roughnessFactor;
		std::wstring m_metallicRoughnessTextureName;
	};
}
