#pragma once

#include "Libraries/Eigen/Geometry"
#include "GameEngine/Memory/UseStorage.h"

namespace GameEngine
{
	class StandardMaterial : public UseStorage<StandardMaterial>
	{
	public:
		static std::size_t GetTextureCount();

	public:
		StandardMaterial();
		StandardMaterial(const std::string& name, const Eigen::Vector4f& baseColor, const std::wstring& albedoMapName);

		const std::string& GetName() const;
		void SetName(const std::string& name);

		const Eigen::Vector4f& GetBaseColor() const;
		void SetBaseColor(const Eigen::Vector4f& baseColor);

		const std::wstring& GetAlbedoMapName() const;
		void SetAlbedoMapName(const std::wstring& albedoMapName);

	private:
		std::string m_name;
		Eigen::Vector4f m_baseColor;
		std::wstring m_albedoMapName;
	};
}
