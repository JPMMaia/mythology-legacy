#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "StandardMaterial.h"
#include "RenderInfo.h"

namespace GameEngine
{
	class InstancedMeshComponent : public BaseComponent
	{
	public:
		explicit InstancedMeshComponent(const std::shared_ptr<StandardMaterial>& material) : 
			m_renderInfo(std::make_shared<RenderInfo>()),
			m_material(material)
		{
		}

	public:
		const std::shared_ptr<StandardMaterial>& GetMaterial() const { return m_material; }
		void SetMaterial(const std::shared_ptr<StandardMaterial>& material) { m_material = material; };

		const std::shared_ptr<RenderInfo>& GetRenderInfo() const { return m_renderInfo; }

	private:
		std::shared_ptr<RenderInfo> m_renderInfo;
		std::shared_ptr<StandardMaterial> m_material;
	};
}
