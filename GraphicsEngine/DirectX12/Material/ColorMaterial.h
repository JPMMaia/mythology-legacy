#pragma once

#include "Interfaces/IMaterial.h"

namespace GraphicsEngine
{
	class ColorMaterial : public IMaterial
	{
	public:
		explicit ColorMaterial(const std::string& name);

		const std::string& Name() const;

		DirectX::XMFLOAT3 Color() const;
		void SetColor(const DirectX::XMFLOAT3& color);

	private:
		std::string m_name;
		DirectX::XMFLOAT3 m_color;
	};
}
