#pragma once

#include "GameEngine/Memory/UseStorage.h"
#include "RenderEngine/Materials/Material.h"

namespace GameEngine
{
	class StandardMaterial : public UseStorage<StandardMaterial>
	{
	public:
		StandardMaterial() = default;
		StandardMaterial(const std::string& name, const RenderEngine::Material& data);

	public:
		const std::string& GetName() const;
		void SetName(const std::string& name);

		const RenderEngine::Material& GetData() const;

	public:
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
		RenderEngine::Material m_data;
	};
}
