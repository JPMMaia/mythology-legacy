#pragma once

#include "GraphicsEngineInterfaces/IMaterial.h"

namespace DirectX12Engine
{
	class ColorMaterial : public GraphicsEngine::IMaterial
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
